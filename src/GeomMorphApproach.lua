-- Diploma work - 2005, Sofia University, department Mathematics & Informatics, speciality Artificial Intelligence
-- Image artifacts removal algorithm using Morphologicaly-Geometric approach
-- Alexander Marinov

require("luaia"); -- loads image analyzer library
require("lfs");   -- loads lua file system library

-- stdandard output wrapper functions
function out(s)
	io.write(s); io.flush();
end
function outln(s)
	io.write(s.."\n"); io.flush();
end

-- calls a callback for each subcontour in contour container
function foreach_contour_in_contours(contours, callback)
	if contours then
		local i, n;
		n=contours:count();
		for i=0,n-1 do
			local contour=contours:get(i);
			if callback(contour) then
				break;
			end
		end
	end
end

-- binary search algorithm for extracting image contours at each binarization level 
-- over distrance transformed input image
function find_tree_function(img_gray, left, right, contours_callback, silent, func_contours)
	local level=left + math.floor((right-left)/2);
	if level>left and level<right then
		local i;
		if not silent then
			out(left.." < "..tostring(level).." > "..right.."             \r");
		end

		local img=img_gray:copy();
		if not func_contours then
			-- on first time call determine minimum and maximum contours
			-- at the most low and the most high binarization levels
			img:binarize_level(left);
			img:inverse();
			local max_contours = img:contours_find();
			local max_contours_count = max_contours:count();
			contours_callback(left, max_contours);
			img=img_gray:copy();
			img:binarize_level(right);
			img:inverse();
			local min_contours = img:contours_find();
			local min_contours_count = min_contours:count();
			contours_callback(right, min_contours);

			-- allocates binarization level to contours count hashmap
			func_contours={};
			func_contours[left]=max_contours_count;
			func_contours[right]=min_contours_count;

			img=img_gray:copy();
		end

		img:binarize_level(level);
		-- inverse the image to convert black (0) to non-zero colors
		img:inverse();
		-- find contours around non-zero pixels in the image
		local contours = img:contours_find();
		local contours_count=contours:count();
		contours_callback(level, contours);

		-- map level number to contour count at this level
		func_contours[level]=contours_count;

		-- if contour count at left level match contour count at this level
		if contours_count == func_contours[left] then
			-- make the levels between them equals
			for i=left,level do
				func_contours[i]=contours_count;
			end
		else
			if left<level then
				-- recurse in left interval
				find_tree_function(img_gray, left, level, contours_callback, silent, func_contours);
			end
		end

		-- if contour count at right level match contour count at this level make
		if contours_count == func_contours[right] then
			-- make the levels between them equals
			for i=level,right do
				func_contours[i]=contours_count;
			end
		else	
			if level<right then
				-- recurse in right interval
				find_tree_function(img_gray, level, right, contours_callback, silent, func_contours);
			end
		end
	end
end

-- Program entry point
function process(sourceFile, outputFile, produce_eval_image)
	-- check for valid arguments
	assert(sourceFile, "Input image is missing");
	assert(outputFile, "Output image is missing");

	outln("load input image "..sourceFile);
	local image=assert(ia.image_load(sourceFile), "Unable to load "..sourceFile);

	outln("convert the main image to gray levels");
	local img_gray=image:convert_gray(ia.UINT_16);

	outln("binarize grayed image");
	img_gray:binarize_otsu();

	outln("determine background/feature colors");
	local w, h=img_gray:get_metrics();
	local histogram={};
	local i,j;
	for i=0,h-1 do
		for j=0,w-1 do
			local color=img_gray:get_pixel(j, i);
			histogram[color]=(histogram[color] or 0)+1;
		end
	end
	local inversed=false;
	local max_color_count=ia.MIN_INT;
	local max_color;
	table.foreach(histogram, function(color, count)
		if max_color_count<count then
			max_color_count=count;
			max_color=color;
		end
	end);
	if max_color == 0 then
		outln("inverse image");
		inversed=true;
		img_gray:inverse();
	end

	-- force adding small artifacts on the image corners
	img_gray:set_pixel(0, 0, 0);
	img_gray:set_pixel(w-1, 0, 0);
	img_gray:set_pixel(w-1, h-1, 0);
	img_gray:set_pixel(0, h-1, 0);

	outln("do distance transform");
	local mask=ia.distance_transform_mask(5, 5, 7, 11, ia.DT_INF, ia.DT_INF);
	local min, max=img_gray:distance_transform_sequential(mask);

	outln("find development tree");
	local contours_hashmap = {}; -- helper hashmap storing the passed levels
	local contours_tree = {};    -- development contours tree
	find_tree_function(img_gray, min+1, max, function (level, contours)
		local contours_count=contours:count();
		if not contours_hashmap[contours_count] then
			contours_hashmap[contours_count] = true;
			table.insert(contours_tree, {level=level, contours=contours});
		end
	end, false);

	outln("sort the development tree by levels by increasing order");
	table.sort(contours_tree, function (a, b)
		if a.level<b.level then return true; end
	end);

	local tree={};
	outln("add initial tree contours and prepare for bottom-up tree generation");
	foreach_contour_in_contours(contours_tree[1].contours, function(contour)
		table.insert(tree, {common_contour=contour});
	end);
	table.remove(contours_tree, 1);

	outln("generate contour tree");
	table.foreachi(contours_tree, function(i, contours_entry)
		foreach_contour_in_contours(contours_entry.contours, function (contour)
			local concat_indexes={};
			table.foreachi(tree, function(j, tree_item)
				if contour:in_contour(tree_item.common_contour) then
					table.insert(concat_indexes, j);
				end
			end);
			if table.getn(concat_indexes)>1 then
				local concat_contours={};
				table.foreachi(concat_indexes, function(k, concat_index)
					table.insert(concat_contours, tree[concat_index-k+1]);
					table.remove(tree, concat_index-k+1);					
				end);
				local new_tree_entry={common_contour=contour, subtree=concat_contours};
				table.insert(tree, new_tree_entry);
			end
		end);
	end);

	function evaluate_tree(t, koef, evaluated_items)
		if t.subtree then
			local n=table.getn(t.subtree);
			table.foreachi(t.subtree, function(_, subitem)
				evaluate_tree(subitem, koef/n, evaluated_items);
			end);
		else
			table.insert(evaluated_items, {t.common_contour, koef});
		end
	end

	outln("create evaluation image");
	local image_eval=ia.image_new(w, h, ia.UINT_8, true);
	image_eval:fill(0);
	evaluated_items={};
	outln("evaluate tree contours");
	evaluate_tree(tree[1], 1.0, evaluated_items);

	-- Remark: The image contours are evaluated and can be sorted by importance
	-- Just for reconstructing the image from contours we are using their evaluation 
	-- factor instead the importance level

	--outln("sort tree contours in level of evaluations");
	--table.sort(evaluated_items, function(a, b)
	--	if a[2]<b[2] then return 1; end
	--end);

	outln("fill evaluated contours over image with colors respective to their evaluation");
	table.foreachi(evaluated_items, function(index, item)
		local contour=item[1];
		local evaluation=item[2];

		-- Remark: Instead the evaluation factor we could use their index in the array,
		-- for calculating the color level

		local color=evaluation * 512; -- scale evaluation to color
		--local color=(index / table.getn(evaluated_items)) * 255;

		if color>255 then color = 255; end

		-- fill contours with calculated color over evaluation image depending of contour 
		-- evaluation factor or the index in the sorted array, 
		-- where an increasing factor from 0 to 255 marks increasing importance
		contour:fill(image_eval, 255-color);
	end);

	outln("supress artifacts according the evaluation image");
	for i=0,h-1 do
		for j=0,w-1 do
			local color=image:get_pixel(j, i);
			local factor=image_eval:get_pixel(j, i);
			if factor == 0 then
				-- draw background color
				color=ia.color(max_color, max_color, max_color);
			else
				local r,g,b;
				r=(ia.red_color(color) * factor)/255;
				g=(ia.green_color(color) * factor)/255;
				b=(ia.blue_color(color) * factor)/255;
				color=ia.color(r, g, b);
			end
			image:set_pixel(j, i, color);
		end
	end
	outln("save suppressed image to "..outputFile);
	image:save(outputFile);

	-- Save the evaluation image if requested
	if produce_eval_image then
		local evalOutputFile=string.gsub(outputFile, "%.([^%.]+)$", "_eval.%1");
		outln("save evaluation image to "..evalOutputFile);
		-- inverse colors to underline the important contours in black
		image_eval:inverse();
		image_eval:save(evalOutputFile);
	end
end

function process_directory(source_directory, output_directory, produce_eval_image)
	local function prepare_directory_name(directory)
		-- replace backslash with slash
		directory=string.gsub(directory, "\\", "/");

		-- remove slash from the end of directory name
		if string.sub(directory, string.len(directory)) == "/" then
			directory=string.sub(directory, 1, string.len(directory)-1);
		end
		return directory;
	end

	source_directory=prepare_directory_name(source_directory);
	output_directory=prepare_directory_name(output_directory);

	for entry in lfs.dir(source_directory) do
		if entry~="." and entry~=".." then
			local ext;
			string.gsub(entry, "%.(%S+)$", function (_ext) 
				ext=string.lower(_ext);
			end);
			if ext == "jpg" or ext == "jpeg" then
				local source_file=source_directory.."/"..entry;
				local output_file=output_directory.."/"..entry;
				outln("-------------------------------------------------------");
				outln("Processing image: "..source_file.." ==> "..output_file);
				outln("-------------------------------------------------------");
				process(source_file, output_file, produce_eval_image);
			end
		end
	end
	outln("\nDone!");
end

process_directory(unpack(lrun.arg));
