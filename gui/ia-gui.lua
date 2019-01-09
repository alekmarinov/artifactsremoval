DATA_DIR="./";
STRUCTURE_ELEMENT="s3x3_box";
-- convolution border type = 0:zeroed 1:periodic 2:mirrored 3:constant 
IA_CONVOLUTION_BORDER_T0=0;
IA_CONVOLUTION_BORDER_T1=1;
IA_CONVOLUTION_BORDER_T2=2;
IA_CONVOLUTION_BORDER_T3=3;
IA_CONVOLUTION_BORDER_TYPE=IA_CONVOLUTION_BORDER_T3;


DEFAULT_NEW_IMAGE_SIZE={100, 100};

function id_first(first)
	__first_id=first;
	return __first_id;
end

function id_next()
	__first_id=__first_id+1;
	return __first_id;
end

IAT_UINT_8             = id_first(0);          IAT_UINT_16            = id_next(); 
IAT_UINT_24            = id_next();            IAT_UINT_32            = id_next();
IAT_INT_8              = id_next();            IAT_INT_16             = id_next();
IAT_INT_24             = id_next();            IAT_INT_32             = id_next();
IAT_FLOAT              = id_next();            IAT_DOUBLE             = id_next();   
IAT_BOOL               = id_next();  

GRAY_COLOR_TYPE        = IAT_UINT_16;

GIA_CTL_WINDOW         = id_first(0);         GIA_CTL_BUTTON         = id_next();
GIA_CTL_EDIT           = id_next();           GIA_CTL_LABEL          = id_next();
GIA_CTL_LISTBOX        = id_next();           GIA_CTL_COMBOBOX       = id_next();
GIA_CTL_GROUPBOX       = id_next();           GIA_CTL_SCROLLBAR      = id_next();
GIA_CTL_SLIDEBAR       = id_next();

IAS_CAPTION            = axtoi("80000000");   IAS_MINIMIZE           = axtoi("40000000");
IAS_MAXIMIZE           = axtoi("20000000");   IAS_SYSMENU            = axtoi("10000000");
IAS_BORDER             = axtoi("08000000");   IAS_DIALOG             = axtoi("04000000");
IAS_MODAL              = axtoi("02000000");   IAS_THICKFRAME         = axtoi("01000000");
IAS_TOPMOST            = axtoi("00800000");   IAS_CHILD              = axtoi("00400000");
IAS_GROUP              = axtoi("00200000");   IAS_TABSTOP            = axtoi("00100000");
IAS_LEFT               = axtoi("00080000");   IAS_RIGHT              = axtoi("00040000");
IABS_CHECKBOX          = axtoi("00020000");   IABS_GROUPBOX          = axtoi("00010000");

IA_ALIGN_LEFT          = 0;
IA_ALIGN_RIGHT         = 1;
IA_ALIGN_CENTER        = 2;

DT_INF                 = axtoi("7FFFFFFF");

function IA_RGB(r, g, b)
	return bit_or(bit_and(r, 255), bit_shl(bit_and(g, 255), 8), bit_shl(bit_and(b, 255), 16));
end

-- File
ID_NEW                 = id_first(1);
ID_OPEN                = id_next();
ID_SAVE                = id_next();
--ID_INFO                = id_next();
ID_QUIT                = id_next();

-- Distance Transform
ID_DT_CITY_BLOCK_S     = id_next();
ID_DT_CITY_BLOCK_P     = id_next();
ID_DT_CHESSBOARD_S     = id_next();
ID_DT_CHESSBOARD_P     = id_next();
ID_DT_CHAMFER_3_4_S    = id_next();
ID_DT_CHAMFER_3_4_P    = id_next();
ID_DT_CHAMFER_5_7_11_S = id_next();
ID_DT_CHAMFER_5_7_11_P = id_next();
ID_DT_CHAMFER_7X7_S    = id_next();
ID_DT_CHAMFER_7X7_P    = id_next();

distance_transform     =
{
	[ID_DT_CITY_BLOCK_S]     = {ia_distance_transform_sequential, {3,  1, DT_INF, DT_INF, DT_INF, DT_INF} },
	[ID_DT_CITY_BLOCK_P]     = {ia_distance_transform_parallel, {3,  1, DT_INF, DT_INF, DT_INF, DT_INF} },
	[ID_DT_CHESSBOARD_S]     = {ia_distance_transform_sequential, {3,  1,      1, DT_INF, DT_INF, DT_INF} },
	[ID_DT_CHESSBOARD_P]     = {ia_distance_transform_parallel, {3,  1,      1, DT_INF, DT_INF, DT_INF} },
	[ID_DT_CHAMFER_3_4_S]    = {ia_distance_transform_sequential, {5,  3,      4, DT_INF, DT_INF, DT_INF} },
	[ID_DT_CHAMFER_3_4_P]    = {ia_distance_transform_parallel, {5,  3,      4, DT_INF, DT_INF, DT_INF} },
	[ID_DT_CHAMFER_5_7_11_S] = {ia_distance_transform_sequential, {5,  5,      7,     11, DT_INF, DT_INF} },
	[ID_DT_CHAMFER_5_7_11_P] = {ia_distance_transform_parallel, {5,  5,      7,     11, DT_INF, DT_INF} },
	[ID_DT_CHAMFER_7X7_S]    = {ia_distance_transform_sequential, {7, 12,     17,     27,     38,     43} },
	[ID_DT_CHAMFER_7X7_P]    = {ia_distance_transform_parallel, {7, 12,     17,     27,     38,     43} },
};

-- Image
ID_GRAY_SCALE     = id_next();
ID_COUNTOURS      = id_next();
ID_FILL                    = id_next();
ID_INVERSE            = id_next();
ID_BINARIZE           = id_next();
ID_DILATION            = id_next();
ID_EROSION           = id_next();
ID_OPENING           = id_next();
ID_CLOSING            = id_next();

-- Window
ID_BINARIZATION        = id_next();
ID_TREE                = id_next();
ID_FIND_PIC            = id_next();
ID_FIND_CENTER         = id_next();
ID_MASSIVE_CONTOURS    = id_next();
ID_CUT_MASSIVE_CONTOURS= id_next();

-- Heuristics
ID_HEURISTIC_1         = id_next();
ID_HEURISTIC_2         = id_next();
ID_HEURISTIC_3         = id_next();

-- Nadia
ID_CONVOLUTION_BY_SECTORS	= id_next();
ID_ALEK_HACK			= id_next();
ID_ALEK_BLUR			= id_next();
ID_NADIA_TEST			= id_next();
ID_NADIA_DRAW_LINE		= id_next();
ID_FIND_MIN_BY_GRADIENT		= id_next();
ID_FIND_MIN_BY_PARTIAL_BIN	= id_next();
ID_FIND_MIN_BY_WHOLE_BIN	= id_next();
ID_FIND_MIN_BY_HIST_BIN		= id_next();
ID_2D_FFT			= id_next();
		

-- Binarization dialog items
IDD_BINARIZE_CHECKBOX = id_first(1);
IDD_BINARIZE_SLIDEBAR = id_next();
IDD_BINARIZE_LABEL_POS= id_next();

function on_file_new(self, id)
	if app.image then
		ia_image_destroy(app.image);
	end

	app.mass_center_x=nil;
	app.mass_center_y=nil;

	if app.contours then
		ia_contours_destroy(app.contours);
		app.contours=nil;
		app.contour_blue=nil;
	end

	app.image=ia_image_new(DEFAULT_NEW_IMAGE_SIZE[1], DEFAULT_NEW_IMAGE_SIZE[2], IAT_UINT_32, false);
	ia_image_fill(app.image, IA_RGB(255, 255, 255));
	gia_win_redraw(self);
end

function on_file_open(self, id, filename)
	filename=filename or gia_win_dialog_file(self, "Jpeg Files (*.jpg)\0*.jpg\0Img Files (*.img)\0*.img\0All Files (*.*)\0*.*\0\0\0", false);
	if filename then
		if app.image then
			ia_image_destroy(app.image);
		end
		if app.contours then
			ia_contours_destroy(app.contours);
			app.contours=nil;
			app.contour_blue=nil;
		end
		app.image=ia_image_load(filename);
		gia_win_redraw(self);
		app.image_file_name=filename;
	end
end

function on_file_save(self, id)
	local filename=gia_win_dialog_file(self, "Jpeg Files (*.jpg)\0*.jpg\0Img Files (*.img)\0*.img\0All Files (*.*)\0*.*\0\0\0", true);
	if filename then
		ia_image_save(app.image, filename)
	end
end

function on_file_info(self, id)
end

function on_file_quit(self, id)
	gia_win_quit(self);
end

function on_dt_sequential(self, id)
	if app.image then
		local mask, dt_function;

		dt_function=distance_transform[id][1];

		mask = ia_distance_transform_mask(unpack(distance_transform[id][2]));

		if id == ID_DT_CITY_BLOCK then
		elseif id == ID_DT_CHESSBOARD then
			mask = ia_distance_transform_mask(3, 1, 1, DT_INF, DT_INF, DT_INF);
		elseif id == ID_DT_CHAMFER_3_4 then
			mask = ia_distance_transform_mask(5, 3, 4, DT_INF, DT_INF, DT_INF);
		elseif id == ID_DT_CHAMFER_5_7_11 then
			mask = ia_distance_transform_mask(5, 5, 7, 11, DT_INF, DT_INF);
		elseif id == ID_DT_CHAMFER_7X7 then
			mask = ia_distance_transform_mask(7, 12, 17, 27, 38, 43);
		end

		local img_gray=ia_image_convert_gray(app.image, GRAY_COLOR_TYPE);

		ia_image_destroy(app.image);
		ia_binarize_otsu(img_gray);
		local min, max=dt_function(img_gray, mask);
		--ia_image_normalize_colors(img_gray, min, max, 0, 255);

		img_rgb=ia_image_convert_rgb(img_gray);
		ia_image_destroy(img_gray);
		ia_image_destroy(mask);
		app.image=img_rgb;
		gia_win_redraw(self);
	end
end

function on_dt_parallel(self, id)
end

function on_window_binarization(self, id)
	if app.binarize_dlg then
		gia_win_destroy(app.binarize_dlg);
	end

	local dlg_template = gia_dialog_template_new("Binarization...", bit_or(IAS_CAPTION, IAS_SYSMENU, IAS_BORDER), 0, 0, 274, 45, 0 );

	gia_dialog_template_add(dlg_template, gia_control_template_new(GIA_CTL_BUTTON,   "on/off", bit_or(IAS_RIGHT, IABS_GROUPBOX), 7,3,260,33, -1));
	gia_dialog_template_add(dlg_template, gia_control_template_new(GIA_CTL_LABEL,    "", 0, 10, 15, 18, 12, IDD_BINARIZE_LABEL_POS));
	gia_dialog_template_add(dlg_template, gia_control_template_new(GIA_CTL_BUTTON,   "", bit_or(IAS_CHILD, IABS_CHECKBOX), 237,3,8,10, IDD_BINARIZE_CHECKBOX));
	gia_dialog_template_add(dlg_template, gia_control_template_new(GIA_CTL_SLIDEBAR, "",      0,  55,13,205,18, IDD_BINARIZE_SLIDEBAR));
	app.binarize_dlg=gia_win_dialog_new_template(self, dlg_template);
	gia_dialog_template_destroy(dlg_template);
	if app.binarize_dlg then
		local sb=gia_win_get_dialog_item(app.binarize_dlg, IDD_BINARIZE_SLIDEBAR);
		gia_slidebar_set_minmax(sb, 0, 255);
		gia_win_set_visible(app.binarize_dlg , true);
	end
end

function on_window_binarization_scroll(self, item)
	local label_pos=gia_win_get_dialog_item(self, IDD_BINARIZE_LABEL_POS);

	app.binarize_level=gia_slidebar_get_pos(item);
	gia_win_set_title(label_pos, tostring(app.binarize_level));
	gia_win_redraw(app.main_window);
end

function on_window_binarization_check(self, item)
	local slider=gia_win_get_dialog_item(self, IDD_BINARIZE_SLIDEBAR);
	app.is_binarize=gia_button_is_checked(item);
	gia_win_set_enable(slider, app.is_binarize);
	gia_win_redraw(app.main_window);
end

function binarize_level_percent2(image, percent)
	if image then
		local _, _, _, is_gray = ia_image_get_metrics(image);
		if is_gray then
			local min, max = ia_image_get_min_max(image);
			local level=min+(max-min)*(percent)/100;
			ia_binarize_level(image, level);
		end
	end
end

function binarize_level_percent(image, level)
	if image then
		local _, _, _, is_gray = ia_image_get_metrics(image);
		if is_gray then
			ia_binarize_level(image, level);
		end
	end
end

function foreach_contour_in_contours(contours, callback)
	if contours then
		local i, n;
		n=ia_contours_count(contours);
		for i=0,n-1 do
			local contour=ia_contours_get(contours, i);
			callback(contour);
		end
	end
end

function foreach_point_in_contour(contour, callback)
	if contour then
		local i, n;
		n=ia_contour_count(contour);
		for i=0,n-1 do
			local x, y = ia_contour_get(contour, i);
			callback( x, y );
		end
	end
end

function on_image_grayscale(self, id)
	if app.image then
		local img_gray=ia_image_convert_gray(app.image, GRAY_COLOR_TYPE);
		ia_image_destroy(app.image);
		app.image=ia_image_convert_rgb(img_gray);
		ia_image_destroy(img_gray);
	end
end

function on_image_contours(self, id)
	if app.image then
		if app.contours then
			ia_contours_destroy(app.contours);
			app.contours=nil;
			app.contour_blue=nil;
		end
		local img_gray=ia_image_convert_gray(app.image, GRAY_COLOR_TYPE);
		binarize_level_percent(img_gray, app.binarize_level);
		ia_image_inverse(img_gray);

		app.contours = ia_contours_find(img_gray);
		ia_image_destroy(img_gray);
		gia_win_redraw(self);
	end
end

function on_image_fill(self, id)
	if app.image and app.contours then
		foreach_contour_in_contours(app.contours, 
			function(contour) 
				ia_contour_fill(contour, app.image, IA_RGB(0, 255, 0));
			end);
		gia_win_redraw(self);
	end
end

function draw_function(x, y, w, h, ...)
	local min_n=9999999;
	local max_n=-9999999;
	local min_v=min_n;
	local max_v=max_n;
	local values_sorted={};
	local degree_n, degree_v;
	local i;
	local markers_n=5;
	local markers_v=4;
	local space_w=50;
	local space_h=20;
	local fx, fy, fw, fh=x+space_w, y, w-space_w, h-space_h;
	local max_degree_n, max_degree_v=10, 5;
	local mod_n, mod_v;

	if app.draw_context then
		gia_draw_context_destroy(app.draw_context);
	end
	app.draw_context=gia_draw_context_new();

	gia_draw_context_add_rect(app.draw_context, x, y, w, h, 0);
	gia_draw_context_add_rect(app.draw_context, fx, fy, fw, fh, IA_RGB(64, 64, 64));

	local min_x, min_y;
	table.foreachi(arg, function(i, f)
		if type(f) == "table" then
			local color=arg[i+1];
			table.foreach(f, function(n, v)
				if max_n<n then max_n=n; end
				if min_n>n then min_n=n; end
				if max_v<v then max_v=v; end
				if min_v>v then min_v=v; end

				table.insert(values_sorted, n);
			end);

			i=1;
			while not (degree_n and degree_v) do
				local pow10=math.pow(10, i);
				local pow10_=math.pow(10, i-1);
				if max_n-min_n<pow10-8*pow10_ and not degree_n then
					degree_n=i-1;
				end
				if max_v-min_v<pow10-8*pow10_ and not degree_v then
					degree_v=i-1;
				end
				i=i+1;
			end

			degree_n=degree_n-1;
			degree_v=degree_v-1;

			mod_n=math.pow(10, degree_n);
			while (max_n-min_n)/mod_n > max_degree_n do
				mod_n=mod_n*2;
			end

			mod_v=math.pow(10, degree_v);
			while (max_v-min_v)/mod_v > max_degree_v do
				mod_v=mod_v*2;
			end
			table.sort(values_sorted);

			local prev_x, prev_y;
			min_x=min_n*fw/(max_n-min_n);
			min_y=min_v*fh/(max_v-min_v);
			table.foreachi(values_sorted, function(i, n)
				local pos_x=n*fw/(max_n-min_n);
				local pos_y=f[n]*fh/(max_v-min_v);
				if prev_x and prev_y then
					gia_draw_context_add_line(app.draw_context, -min_x+fx+prev_x, min_y+fy+fh-prev_y, -min_x+fx+pos_x, min_y+fy+fh-pos_y, color);
					if math.mod(n, mod_n) == 0 then
						gia_draw_context_add_line(app.draw_context, -min_x+fx+pos_x, fy+fh, -min_x+fx+pos_x, fy+fh-10, IA_RGB(255, 255, 255));
						if -min_x+pos_x>50 then
							gia_draw_context_add_text(app.draw_context, ""..n, -min_x+fx+pos_x, fy+fh, IA_RGB(255, 255, 255), 16, IA_ALIGN_RIGHT);
						end
					end
				end
				prev_x, prev_y=pos_x, pos_y;
			end);
		end
	end);
	for i=min_v, max_v do
		local pos_y=i*fh/(max_v-min_v);
		if math.mod(i, mod_v) == 0 then
			gia_draw_context_add_line(app.draw_context, fx, min_y+fy+fh-pos_y, fx+20, min_y+fy+fh-pos_y, IA_RGB(255, 255, 255));
			gia_draw_context_add_text(app.draw_context, ""..i, fx, min_y+fy+fh-pos_y, IA_RGB(255, 255, 255), 16, IA_ALIGN_RIGHT);
		end
	end
	gia_draw_context_add_line(app.draw_context, fx, fy, fx, fy+fh, IA_RGB(255, 255, 255));
	gia_draw_context_add_line(app.draw_context, fx, fy+fh, fx+fw, fy+fh, IA_RGB(255, 255, 255));
end

function on_window_tree2(self, id)
	if app.tree_win then
		gia_win_destroy(app.tree_win);
	end
	local dlg_template = gia_dialog_template_new("Contour Development Tree...", 
		bit_or(IAS_CAPTION, IAS_SYSMENU, IAS_BORDER), 0, 0, 300, 50, 0 );
	local rect_l, rect_t, rect_r, rect_b = gia_win_get_win_rect(app.main_window);

	app.tree_win=gia_win_child(self, rect_l, rect_b, rect_r-rect_l, 150, nil, bit_or(IAS_CAPTION, IAS_SYSMENU, IAS_BORDER));

	if app.tree_win then
		gia_win_set_visible(app.tree_win, true);
		local f={};
		math.randomseed(1234);
		
		for i=-360,360 do
			local v=i*math.pi/180;
			f[i]=math.cos(v);
		end
		app.func=f;
	end
end

function on_window_tree(self, id)
	if app.tree_win then
		gia_win_destroy(app.tree_win);
	end
	local rect_l, rect_t, rect_r, rect_b = gia_win_get_win_rect(app.main_window);

	app.tree_win=gia_win_child(self, rect_l, rect_b, rect_r-rect_l, 150, nil, bit_or(IAS_CAPTION, IAS_SYSMENU, IAS_BORDER));

	if app.tree_win then
		gia_win_set_visible(app.tree_win, true);
		local sx, sy;
		local sx2, sy2;
		local rect_l2, rect_t2, rect_r2, rect_b2 = gia_win_get_rect(app.tree_win);
		local w, h, f, g = ia_image_get_metrics(app.image);
		sx=(rect_r-rect_l)/w;
		sy=(rect_b-rect_t)/h;

		-- construct tree
		local contours_max;
		local mask=ia_distance_transform_mask(5, 5, 7, 11, DT_INF, DT_INF);
		local img_gray=ia_image_convert_gray(app.image, GRAY_COLOR_TYPE);
		ia_binarize_otsu(img_gray);
		local min, max = ia_distance_transform_sequential(img_gray, mask);
		min=min+1;
		ia_image_destroy(mask);
		local contours_count_max;
		local level;
		local f, f2 = {}, {};
		for level=min,max do
			local img=ia_image_copy(img_gray);
			ia_binarize_level(img, level);
			ia_image_inverse(img);
			local contours = ia_contours_find(img);
			ia_image_inverse(img);
			local imgrgb=ia_image_convert_rgb(img);

			local contours_count=ia_contours_count(contours);

			foreach_contour_in_contours(contours, function(contour)
				foreach_point_in_contour(contour, function(x, y)
					ia_image_set_pixel(imgrgb, x, y, IA_RGB(255, 255, 0));
				end);
			end);

			if level==min then
				contours_count_max=contours_count;
				sx2=(rect_r2-rect_l2)/(max-min);
				sy2=(rect_b2-rect_t2)/contours_count_max;
			end
		
			gia_win_draw_image(app.main_window, imgrgb, 0, 0, sx, sy);

			ia_contours_destroy(contours);
			ia_image_destroy(img);
			ia_image_destroy(imgrgb);

			f[level]=contours_count;
			f2[level]=math.abs( ( f[level - 1] and contours_count - f[level - 1]) or 0 );
			if contours_count == 1 then
				break;
			end
		end
		ia_image_destroy(img_gray);

		--math.randomseed(1234);
		
		--for i=-360,360 do
		--	local v=i*math.pi/180;
		--	f[i]=math.cos(v);
		--end
		if app.draw_context then
			gia_draw_context_destroy(app.draw_context);
		end
		app.draw_context=nil;
		app.func=f;
		--app.func2=f2;
		gia_win_redraw(app.tree_win);
	end

end

function calculate_derivate(F, Xmin, Xmax)
	local deriv={};
	local i;
	Xmin=Xmin or 1;
	Xmax=Xmax or table.getn(F);
	for i=Xmin, Xmax do
		if F[i] then
			deriv[i]=math.abs( ( F[i - 1] and F[i] - F[i - 1]) or 0 );
		end
	end
	return deriv;
end

function arg_max(F)
	local max=-999999;
	local max_a=0;
	table.foreachi(F, function(a, value)
		if value>max then
			max=value;
			max_a=a;
		end
	end);
	return max_a-1;
end

function on_window_tree(self, id)
	
	function find_tree_function(img_gray, left, right, func_contours, sx, sy)
		local level=left + math.floor((right-left)/2);
		if level>left and level<right then
			local i;
			gia_win_set_title(self, left.." < "..tostring(level).." > "..right);

			local img=ia_image_copy(img_gray);
			ia_binarize_level(img, level);
			ia_image_inverse(img);
			local contours = ia_contours_find(img);
			ia_image_inverse(img);

			local imgrgb=ia_image_convert_rgb(img);
			gia_win_draw_image(app.main_window, imgrgb, 0, 0, sx, sy);
			ia_image_destroy(imgrgb);

			local contours_count=ia_contours_count(contours);
			ia_contours_destroy(contours);
			ia_image_destroy(img);
			func_contours[level]=contours_count;
			if contours_count == func_contours[left] then
				for i=left,level do
					func_contours[i]=contours_count;
				end
			else
				if left<level then
					find_tree_function(img_gray, left, level, func_contours, sx, sy);
				end
			end

			if contours_count == func_contours[right] then
				for i=level,right do
					func_contours[i]=contours_count;
				end
			else	
				if level<right then
					find_tree_function(img_gray, level, right, func_contours, sx, sy);
				end
			end
		end
	end

	if app.tree_win then
		gia_win_destroy(app.tree_win);
	end
	local rect_l, rect_t, rect_r, rect_b = gia_win_get_win_rect(app.main_window);

	app.tree_win=gia_win_child(self, rect_l, rect_b, rect_r-rect_l, 150, nil, bit_or(IAS_CAPTION, IAS_SYSMENU, IAS_BORDER));

	if app.tree_win then
		gia_win_set_visible(app.tree_win, true);
		local sx, sy;
		local rect_l2, rect_t2, rect_r2, rect_b2 = gia_win_get_rect(app.tree_win);
		local w, h, f, g = ia_image_get_metrics(app.image);
		sx=(rect_r-rect_l)/w;
		sy=(rect_b-rect_t)/h;

		-- construct tree
		local contours_max;
		local mask=ia_distance_transform_mask(5, 5, 7, 11, DT_INF, DT_INF);
		local img_gray=ia_image_convert_gray(app.image, GRAY_COLOR_TYPE);
		ia_binarize_otsu(img_gray);
		local min, max = ia_distance_transform_sequential(img_gray, mask);
		min=min+1;
		ia_image_destroy(mask);
		local f, f2 = {}, {};

		-- find maximum contours
		local img=ia_image_copy(img_gray);
		ia_binarize_level(img, min);
		ia_image_inverse(img);
		local contours = ia_contours_find(img);
		local contours_count_max=ia_contours_count(contours);
		ia_contours_destroy(contours);
		ia_image_destroy(img);
		f[min]=contours_count_max;
		f[max]=1;


		local title=gia_win_get_title(self);
		find_tree_function(img_gray, min, max, f, sx, sy);
		gia_win_set_title(self, title);

		-- find 1st derivate
		local level;
		local once=true;
		for level=min,max do
			if not once and f[level] == 1 then
				f[level]=nil;
			end
			once=false;
		end

		f2=calculate_derivate(f, min, max);

		ia_image_destroy(img_gray);

		if app.draw_context then
			gia_draw_context_destroy(app.draw_context);
		end

		app.draw_context=nil;
		app.func=f;
		app.func2=f2;
		gia_win_redraw(app.tree_win);
	end

end

function on_window_find_pic(self, id)
	if app.func2 then
		local max_level = arg_max(app.func2);

		on_dt_sequential(self, ID_DT_CHAMFER_5_7_11_S);

		app.binarize_level=max_level+2;
		app.is_binarize = true;
		gia_win_redraw(app.tree_win);

	end
end

function on_window_find_center(self, id)
	if app.image then
		local img_gray=ia_image_convert_gray(app.image, IAT_UINT_8);
		ia_image_inverse(img_gray);
		local img_w, img_h, _, _ = ia_image_get_metrics(img_gray);
		local i,j;
		local sum_j, sum_i, sum_c=0, 0, 0;
		for i=0,img_h-1 do
			for j=0,img_w-1 do
				local col=(ia_image_get_pixel(img_gray, j, i)/255);
				sum_c=sum_c+col;
				sum_j=sum_j + j*col;
				sum_i=sum_i + i*col;
			end
		end
		app.mass_center_x=math.floor(sum_j/sum_c);
		app.mass_center_y=math.floor(sum_i/sum_c);
		ia_image_destroy(img_gray);
	end
end

function contour_mass_center(contour)
	local sum_x, sum_y = 0, 0;
	local count=ia_contour_count(contour);
	foreach_point_in_contour(contour, function(x, y)
		sum_x=sum_x + x;
		sum_y=sum_y + y;
	end);
	return math.floor(sum_x/count), math.floor(sum_y/count);
end

function contour_weight(img, contour)
	local s  = ia_contour_fill(contour, img, IA_RGB(0, 255, 0));
	local p  = ia_contour_count(contour);
	local cx, cy = contour_mass_center(contour);

	local OM2 = (app.mass_center_x-cx)*(app.mass_center_x-cx) + (app.mass_center_y-cy)*(app.mass_center_y-cy);

	if OM2>1 then
		return math.sqrt( s ) / OM2;
	end
end

function on_window_massive_contours(self, id)
	app.func={};
	if app.contours then
		if not app.mass_center_x or not app.mass_center_y then
			on_window_find_center(self);
		end
		local img=ia_image_copy(app.image);
		local contours_indeces={};
		foreach_contour_in_contours(app.contours, function(contour)

			local R = contour_weight(img, contour);

			if R then
				table.insert(app.func, R);
				table.insert(contours_indeces, table.getn(app.func));
			end
		end);

		table.sort(contours_indeces, function (a, b) if app.func[a]>app.func[b] then return true; end end);
		table.sort(app.func, function (a, b) if a>b then return true; end end);

		table.remove(app.func, 1);
		local idx_max=table.remove(contours_indeces, 1);

		app.func2 = calculate_derivate(app.func);

		local d2 = {};
		local sum=0;
		table.foreachi(app.func2, function(i, v) 
			sum = sum + v * v;
		end);

		sum=sum/table.getn(app.func2);

		local arg_max_n;
		table.foreachi(app.func2, function(i, v) 
			if v >= sum then
				arg_max_n=i;
				return true;
			end
		end);
		arg_max_n=(arg_max_n or 2);

		--print("countours = ", table.getn(app.func2), "ARG MAX (dF) = ", arg_max_n, "sum2=", sum);

		local i;
		local contour;

		contour=ia_contours_get(app.contours, idx_max-1);
		ia_contour_fill(contour, app.image, IA_RGB(0, 255, 0));

		app.massive_contours={};
		table.insert(app.massive_contours, contour);
		for i=1, arg_max_n do
			contour=ia_contours_get(app.contours, contours_indeces[i]-1);
			ia_contour_fill(contour, app.image, IA_RGB(0, 255, 0));
			table.insert(app.massive_contours, contour);
		end

		ia_image_destroy(img);

		if app.draw_context then
			gia_draw_context_destroy(app.draw_context);
			app.draw_context=nil;
		end

		if app.tree_win then
			gia_win_redraw(app.tree_win);
		end

		-- reset app state
		app.is_binarize=false;

		gia_win_redraw(self);
	end
end

function on_window_cut_massive_contours(self, id)
	local old_contours=app.contours;
	app.contours=nil;
	on_file_open(app.main_window, nil, app.image_file_name);
	app.contours=old_contours;

	local img_w, img_h, img_f, img_g = ia_image_get_metrics(app.image);
	local img=ia_image_new(img_w, img_h, IAT_UINT_8, true);
	table.foreachi(app.massive_contours, function(i, contour)
		--print(i);
		ia_contour_fill(contour, img, 255);

		foreach_point_in_contour(contour, function(x, y)
			ia_image_set_pixel(img, x, y, 255);
		end);
	end);

	if true then
		local i,j;
		for i=0,img_h-1 do
			for j=0,img_w-1 do
				if ia_image_get_pixel(img, j, i) == 0 then
					ia_image_set_pixel(app.image, j, i, IA_RGB(255, 255, 255));
				end
			end
		end
	end
	ia_image_destroy(img);
	ia_contours_destroy(app.contours);
	app.contours=nil;
	gia_win_redraw(self);
end

function on_image_inverse(self, id)
	if app.image then
		ia_image_inverse(app.image );
		gia_win_redraw(self);
	end
end

function on_image_binarize(self, id)
	if app.image then
		local image=ia_image_convert_gray(app.image, IAT_UINT_8);
		ia_image_destroy(app.image);
		ia_binarize_level(image, app.binarize_level);
		app.image=ia_image_convert_rgb(image);
		ia_image_destroy(image);
	end
end

function on_image_dilation(self, id)
	local element_rgb=ia_image_load(DATA_DIR.."structures/"..STRUCTURE_ELEMENT..".img");
	local element=ia_image_convert_gray(element_rgb, IAT_UINT_8);
	ia_image_destroy(element_rgb);
	ia_image_inverse(element);
	local image=ia_image_convert_gray(app.image, IAT_UINT_8);
	ia_image_inverse(image);
	ia_binarize_otsu(image);

	local output=ia_morphology_dilation(image, element);

	ia_image_destroy(app.image);
	ia_image_destroy(element);
	ia_image_inverse(output);
	app.image=ia_image_convert_rgb(output);
	ia_image_destroy(output);
	gia_win_redraw(self);
end


function on_image_erosion(self, id)
	local element_rgb=ia_image_load(DATA_DIR.."structures/"..STRUCTURE_ELEMENT..".img");
	local element=ia_image_convert_gray(element_rgb, IAT_UINT_8);
	ia_image_destroy(element_rgb);
	ia_image_inverse(element);
	local image=ia_image_convert_gray(app.image, IAT_UINT_8);
	ia_image_inverse(image);
	ia_binarize_otsu(image);

	local output=ia_morphology_erosion(image, element);

	ia_image_destroy(app.image);
	ia_image_destroy(element);
	ia_image_inverse(output);
	app.image=ia_image_convert_rgb(output);
	ia_image_destroy(output);
	gia_win_redraw(self);
end

function on_image_opening(self, id)
	on_image_erosion(self, id);
	on_image_dilation(self, id);
end

function on_image_closing(self, id)
	on_image_dilation(self, id);
	on_image_erosion(self, id);
end

function on_mouseclick(self, x, y, is_left, is_down)
	if app.image and is_left then
		local img_w, img_h, img_f, img_g = ia_image_get_metrics(app.image);
		local l, t, r, b = gia_win_get_rect(self);
		local sx=(r-l)/img_w;
		local sy=(b-t)/img_h;

		x=x/sx;
		y=y/sy;

		if is_down then
			if bit_and(ia_image_get_pixel(app.image, x, y), 255) == 255 then
				ia_image_set_pixel(app.image, x, y, 0);
			else
				ia_image_set_pixel(app.image, x, y, axtoi("FFFFFFFF"));
			end
		end

		--print("x="..x, "y="..y);
		
		
		if not app.mass_center_x then
			on_window_find_center();
		end

		local center_x=app.mass_center_x+1;
		local center_y=app.mass_center_y+1;
		app.polar_mapping=app.polar_mapping or {};
		app.polar_mapping.current_line={};
		ia_line_draw(center_x, center_y, x, y, "on_image_polar_mapping_line_callback");
		local s="";
		for i,v in app.polar_mapping.current_line do 
			s=s..","..255 - bit_and(ia_image_get_pixel(app.image, v[1],v[2]), 255);
		end
		--print(s);
		--print("------------------");
		
		gia_win_redraw(self);
		app.paint_mode=is_down;
	end
end

function on_mousemove(self, x, y)
	if app.image then
		local is_redraw=false;
		local img_w, img_h, img_f, img_g = ia_image_get_metrics(app.image);
		local l, t, r, b = gia_win_get_rect(self);
		local sx=(r-l)/img_w;
		local sy=(b-t)/img_h;
		x=x/sx;
		y=y/sy;
		if app.paint_mode then
			is_redraw=true;
			ia_image_set_pixel(app.image, x, y, 0);
		else
			app.contour_blue=nil;
			app.old_contour_blue=tostring(app.contour_blue) or "";
			foreach_contour_in_contours(app.contours, 
				function(contour)
					local in_contour=ia_contour_point_in(contour, x, y);
					if in_contour>=0 then
						app.contour_blue=contour;
					end
				end);
			if (app.old_contour_blue or "") ~= (tostring(app.contour_blue) or "") then
				if type(app.old_contour_blue)=="userdata" then
					--print( string.format("%2.5f", contour_weight( app.image, app.contour_blue ) or 0 ) );
					if app.old_contour_blue then
						ia_contour_fill(app.old_contour_blue , app.image, IA_RGB(0, 0, 0));
					end
				end
				is_redraw=true;
			end
		end
		if is_redraw then
			gia_win_redraw(self);
		end
	end
end

function on_paint(self)
	if self == app.tree_win then
		if app.func then
			if not app.draw_context then
				local l,t,r,b=gia_win_get_rect(app.tree_win);
				draw_function(l,t,r,b, app.func, IA_RGB(0, 255, 0), app.func2, IA_RGB(255, 0, 0));
			end
			gia_draw_context_draw(app.draw_context, app.tree_win);
		end
	elseif self == app.main_window and app.image then
		local sx, sy, img;
		local rect_l, rect_t, rect_r, rect_b = gia_win_get_rect(self);
		local w, h, f, g = ia_image_get_metrics(app.image);
		sx=(rect_r-rect_l)/w;
		sy=(rect_b-rect_t)/h;

		if app.is_binarize then
			local img_gray=ia_image_convert_gray(app.image, GRAY_COLOR_TYPE);

			binarize_level_percent(img_gray, app.binarize_level);
			img=ia_image_convert_rgb(img_gray);
			ia_image_destroy(img_gray);
		else
			img=ia_image_copy(app.image);
		end

		if app.contours then
			foreach_contour_in_contours(app.contours, 
				function(contour)
					foreach_point_in_contour(contour, 
						function(x, y)
							ia_image_set_pixel(img, x, y, IA_RGB(255, 0, 0));
						end);
				end);
			foreach_point_in_contour(app.contour_blue, 
				function(x, y)
					ia_image_set_pixel(img, x, y, IA_RGB(0, 0, 255));
				end);

			if app.contour_green then
				foreach_point_in_contour(app.contour_green, 
					function(x, y)
						ia_image_set_pixel(img, x, y, IA_RGB(0, 255, 0));
					end);
			end
			
		end

		if app.mass_center_x and app.mass_center_y then
			local i;
			for i=0,2 do
				-- ia_image_set_pixel(img, app.mass_center_x + i, app.mass_center_y, IA_RGB(0, 0, 255));
			end
			for i=0,2 do
				-- ia_image_set_pixel(img, app.mass_center_x, app.mass_center_y + i, IA_RGB(0, 0, 255));
			end
		end

		gia_win_draw_image(self, img, 0, 0, sx, sy);
		ia_image_destroy(img);
	end
end

function on_close(self)
	if self == app.main_window then
		on_file_quit(self);
	else
		gia_win_destroy(self);
	end
end

function on_destroy(self)
	if self == app.binarize_dlg then
		app.binarize_dlg=nil;
	elseif self == app.tree_win then
		app.tree_win=nil;
	end
end

-- heuristics
function on_heuristic_1(self, id)
	-- heuristic 1
	if id == ID_HEURISTIC_1 then

		if false then
			on_dt_sequential(self, ID_DT_CHAMFER_5_7_11_S);

			local img_gray=ia_image_convert_gray(app.image, GRAY_COLOR_TYPE);
			ia_binarize_level(img_gray, 15);
			app.image=ia_image_convert_rgb(img_gray);
			gia_win_redraw(self);
		else
			on_window_tree(self, id);
			on_window_find_pic(self, id);
			--on_image_erosion(self, id);
		end

		if true then
			on_image_contours(self, id);
			on_window_find_center(self, id);
			on_window_massive_contours(self, id);
			on_window_cut_massive_contours(self, id);
		end
	elseif id == ID_HEURISTIC_2 then
		
	elseif id == ID_HEURISTIC_3 then

	end
end
---------- START NADIA
function on_image_polar_mapping_line_callback(x, y)
	table.insert(app.polar_mapping.current_line, {x, y});
end

function generate_filter_rectangle(m)
	local t={};
	local c=1/m;
	while m>0 do t[m]=c; m=m-1; end
	return t;
end

function generate_filter_triangle(m)
	assert(math.mod(m, 2)==0, "m should be even number");

	local t={};
	local c=m/2;
	local denom = 1/(c*c);

	-- effective formula 2/m * (1 - |x|/ (m/2)) = 1/c*(1 - x/c) = 1/c^2 * (c-x) -> so each part of multiplication is integer
	-- region -c <= x >= c
	for i=c, 0, -1 do
		t[c-i+1]=(c-i)*denom;
	end
	for i=1,c do
		t[m+2-i]=t[i];
	end

	return t;
end

function gen_gausian_sigma(size, sigma)
	local c, m;

	if sigma then
		c=math.floor(3.5*sigma + 0.5);
		m=2*c+1;
	elseif size then
		m = math.ceil(math.mod(size,2)==0 and (size+1) or size);
		c=math.floor(m/2);
		sigma=tonumber(string.format("%.3f", c/3.5));
	end
	return m,c,sigma;
end

function generate_filter_gausian(size, s)
	local m,c,sigma=gen_gausian_sigma(size, s);
	
	local coef=1/(sigma*math.sqrt(2*math.pi));
	local t={};

	local sum=0;
	for i=c, 0, -1 do
		t[c-i+1]=math.exp(-(i*i)/(2*sigma*sigma))*coef;
	end

	local j=c+2;
	for i=c,1,-1 do
		t[j]=t[i];
		j=j+1;
	end
	
	--normalize coefficient to sum up to 1
	--local s=0;
	--for i,v in t do s=s+v; end
	--for i,v in t do t[i]=t[i]/s; end

	return t;
end

function generate_filter_gausian_1derivative(size, sigma)
	local m,c,sigma=gen_gausian_sigma(size, s);
		
	local sigma2=sigma*sigma;
	local coef=-1/(sigma2*sigma*math.sqrt(2*math.pi));
	local t={};

	local sum=0;
	for i=c, 0, -1 do
		t[c-i+1]=-i*math.exp(-(i*i)/(2*sigma2))*coef;
	end

	local j=c+2;
	for i=c,1,-1 do
		t[j]=-t[i];
		j=j+1;
	end
	return t;
end

function generate_filter_gausian_2derivative(size, sigma)
	local m,c,sigma=gen_gausian_sigma(size, s);

	local sigma2=sigma*sigma;
	local coef=1/(sigma2*sigma*math.sqrt(2*math.pi));

	local t={};

	local sum=0;
	for i=c, 0, -1 do
		t[c-i+1]=coef*((i*i)/sigma2 - 1)*math.exp(-(i*i)/(2*sigma2));
	end

	local j=c+2;
	for i=c,1,-1 do
		t[j]=t[i];
		j=j+1;
	end
	return t;
end

function generate_gausian_parametrized(param, N)
	assert(param>=0 and param<=1, "Param should be in [0,1] range");
	local max=1.5*N;
	local min=6;

	local size=math.floor(min + (max-min)*param);
	return generate_filter_gausian(size);
end

function find_min_by_partial_bin(t, max_loc, reversed, isWhole, threshold)
	local three_qrts=table.getn(t)*3/4;
	local two_qrts=table.getn(t)/2;
	local s=0;
	local min_index;
	local count=0;
	local f={};
	
	if isWhole then
		for i,v in t do 
			s=s+v;
		end;
		s=s/table.getn(t);
	end

	if reversed then
		if not isWhole and not threshold then
			for i=max_loc-1, three_qrts, -1 do
				s=s+t[i];
				count=count+1;
			end
			s=s/count;
		elseif threshold then s=threshold; end

		f={};
		for i=max_loc-1, three_qrts, -1 do 
			if t[i]>=s then table.insert(f, 1);
			else table.insert(f, 0); end
		end
	else
		if not isWhole and not threshold then
			for i=max_loc+1, table.getn(t) do
				s=s+t[i];
				count=count+1;
			end
			s=s/count;
		elseif threshold then s=threshold; end
	
		f={};
		for i=max_loc+1, table.getn(t) do 
			if t[i] and t[i]>=s then table.insert(f, 1); --object
			else table.insert(f, 0); end  --background
		end
	end
	
	local extr={};
	for i=1, table.getn(f) do
		local b=i;
		while f[i+1] and f[i+1]==f[i] do i=i+1; end
		local e=i;

		if f[i]==0 then table.insert(extr, {b, e}); end
	end


	local maxL=0;
	local pair={};
	for i,v in extr do
		local l=v[2]-v[1]+1;
		if l>maxL then
			maxL=l;
			pair=v;
		end
	end
	
	min_index=pair[1] and math.floor(pair[1] + (pair[2]-pair[1] +1)/2);
	if min_index then
		if not reversed then
			min_index=max_loc+min_index;
			if min_index> table.getn(t) then
				min_index=table.getn(t);
			end
		else min_index=max_loc - min_index; end
	else min_index=table.getn(t)/2; end

	return min_index;
end
 
function find_min_by_whole_bin(t, max_loc, reversed)
	return find_min_by_partial_bin(t, max_loc, reversed, true);
end

function find_min_by_max_gradient(t, max_loc, reversed)
	local max=0;
	local pos=math.floor(max_loc + (table.getn(t)-max_loc)/2);
	if reversed then
		local three_qrt=table.getn(t)*3/4;
		for i=max_loc-1, three_qrt, -1 do
			if (t[i]-t[i-1])>max then
				max=t[i]-t[i-1];
				pos=i;
			end
		end
	else
		for i=max_loc+1, table.getn(t)-1 do
			if (t[i]-t[i+1])>max then
				max=t[i]-t[i+1];
				pos=i;
			end
		end
	end
	return pos;
end

function find_min_by_histogram_bin(t, max_loc, reversed, filter_size, pixels)
	filter_size=filter_size or table.getn(pixels);
	local min_index=reversed and table.getn(pixels)/2 or (max_loc + (table.getn(pixels) - max_loc)/2);
	local m=math.floor(filter_size/2);
	if m<6 then
		m=6;
	end
	local mask=generate_filter_gausian(m);
	local mask2=generate_filter_gausian_2derivative(m);

	local res=ia_convolution(pixels, mask, 0, 0, IA_CONVOLUTION_BORDER_TYPE);
	local res2=ia_convolution(pixels, mask2, 0, 1, IA_CONVOLUTION_BORDER_TYPE);
	
	if reversed then
		local i=max_loc-1;
		local found=false;
		while i>table.getn(pixels)/2 and not found do
			if res2[i]>0 then
				if res2[i]>res2[i-1] and res2[i]>res2[i+1]then
					found=true;
					min_index=i;
				else i=i-1; end
			else i=i-1; end
		end
	else
		local i=max_loc+1;
		local found=false;
		local s="";
		
		while i<table.getn(pixels) and not found do
			if res2[i]>0 then
				if res2[i]>res2[i-1] and res2[i]>res2[i+1]then
					found=true;
					min_index=i;
				else i=i+1; end
			else i=i+1; end
		end
	end

	return math.floor(min_index);
end

function locate_contour(pixelss, line, ID)
	local verbose=false;
	if verbose then
		local s="";
		for i,v in pixelss do s=s..","..v; end
		print(s);
	end

	local three_qrts=table.getn(pixelss)*3/4;
	local two_qrts=table.getn(pixelss)/2;
	local mult=2.0;
	local size=math.floor(mult*table.getn(pixelss));
	local opt_mask_sizes={};
	local prev_extrema={};
	local irregular_extrema={};

	function find_best_choice_min_area(t, max_loc, reversed, filter_size)
		return find_min_by_partial_bin(t, max_loc, reversed);
	end

	function find_min_index(extrema)
		local min_index;
		table.foreachi(extrema, function(j, v)
			if v[2]==0 then
				min_index=v[1];
				return 1;
			end
		end);
		return min_index;
	end

	function find_least_irregular_extrem()
		local count;
		local size;
		local pos=0; 
		table.foreachi(irregular_extrema, function(i,v)
			if count then
				if v[1]<count then
					count=v[1]; 
					size=v[2];
					pos=i;
				end
			else
				count=v[1]; 
				size=v[2];
				pos=i;
			end
		end);
		
		return irregular_extrema[pos][3][math.ceil(count/2)][1];
	end

	function choose_filter_length(from, to, reps)
		if verbose then
			print("LINE=", line);
		end
		local maxRepetitions=5;
		
		local m=math.floor(from + (to-from)/2);
		if m<=5 then -- size smaller than 6 gives unreasanable (low) sigma
			return choose_filter_length(6, 6, 0);
		end
		
		local mask=generate_filter_gausian(m);
		local t, extrema, min_index, extr_count;

		t=ia_convolution(pixelss, mask, 0, 0, 3);
		extr_count, extrema =count_extrema(t);
		
		if verbose then
			print("PIXELS = ", table.getn(t),"-----------------------", from, to, m);
		end
		if extr_count<=3 then -- check valid states
			if verbose then
				print("extr_count=", extr_count);
			end
			local min, max, min_count, max_count, max_loc, min_loc;
			-- locate max value 
			table.foreachi(extrema, function(i,v)
				if verbose then
					print(i, v[1], v[2]);
				end
				if v[2]==0 then
					if verbose then
						print("MIN = ", t[v[1]], "loc=", v[1]);
					end
					min=t[v[1]];
					min_loc=v[1];
					min_count=(min_count or 0) + 1;
				elseif v[2]==1 then
					if max and t[v[1]]>max then
						max=t[v[1]];
						max_loc=v[1];
					elseif not max then
						max=t[v[1]];
						max_loc=v[1];
					end
					
					if verbose then
						print("MAX= ", max, "loc=",max_loc);
					end
					max_count=(max_count or 0)+1;
				end
			end);

			if max_count==1 and not min_count then 
				if t[1]==max and t[table.getn(t)]<max then -- case2 Fb=Fmax>Fe
					min_index=find_best_choice_min_area(t, max_loc, false, m);
					table.insert(extrema, {min_index, 0});
					if verbose then						
						print("CASE 2", min_index, m);
					end
				elseif	t[1]<max and extrema[1][1]>three_qrts then --special case of Fb<Fmax~Fe -- an artifact.
					min_index=find_best_choice_min_area(t, max_loc, true, m); --reversed
					table.insert(extrema, {min_index, 0});
					if verbose then
						print("CASE 3", min_index, m);
					end
				elseif t[1]<max and t[table.getn(t)]<max then -- case1 Fb<Fmax>Fe
					min_index=find_best_choice_min_area(t, max_loc, false, m);
					table.insert(extrema, {min_index, 0});
					if verbose then
						print("CASE 1", min_index, m);
						for i,v in extrema do print(i, v[1], v[2]); end
					end
				else
					print("UNEXPECTED case", t[1], max, t[table.getn(t)], extrema[1][1], max_loc);
					for i,v in t do print(i,v); end

					min_index=find_best_choice_min_area(t, max_loc, false, m);
					table.insert(extrema, {min_index, 0});
				end
			elseif max_count==2 and min_count==1 then
				if extrema[1][2]==1 then
					if extrema[1][1]==1 and extrema[3][1]>three_qrts and extrema[2][1]>two_qrts then -- case4 max>min<max Fb=max>min<max~Fe
						if verbose then
							print("CASE4");
							local s="";
							for i,v in t do s=s..","..v; end
							print("????", s);
						end
					elseif t[extrema[1][1]]>t[1] and extrema[2][2]==0 and min<max 
					and extrema[3][1]>three_qrts and extrema[1][1]<three_qrts and extrema[2][1]>two_qrts then -- case5 max>min<max Fb<Fmax>min<Fe
						if verbose then
							print("CASE5");
						end
					else
						if verbose then print("3 extrema but no valid conditions"); end
						if table.getn(prev_extrema)>0 then
							if verbose then
								for i,v in prev_extrema[table.getn(prev_extrema)] do
									for j,k in v do print(j, k); end
								end
								print("!!!!!!!!!!!!!",find_min_index(prev_extrema[table.getn(prev_extrema)]));
							end
							return find_min_index(prev_extrema[table.getn(prev_extrema)]),opt_mask_sizes[table.getn(prev_extrema)];
						else
							if reps>=5 then
								if verbose then
									print("enough empty repetitions already", reps);
								end
								max_loc=extrema[3][1];
								extrema={{max_loc, 1}};
								min_index=find_best_choice_min_area(t, max_loc, false, m);
								table.insert(extrema, {min_index, 0});
								-- return choose_filter_length(to, 2*to, 0);
							else
								if verbose then
									print("Not enough empty repetitions yet", reps);
								end
								return choose_filter_length(m, to, reps+1);
							end
						end
					end
				end

			else
				if verbose then
					print("An unexpected case occured");
				end
				if table.getn(prev_extrema)>0 then
					return find_min_index(prev_extrema[table.getn(prev_extrema)]),opt_mask_sizes[table.getn(prev_extrema)];
				else
					if reps>=5 and extr_count==0 then
						return math.floor(table.getn(t)/2);
					else
						return choose_filter_length(m, to, reps+1);
					end
				end
			end
				
			table.insert(opt_mask_sizes, m);
			table.insert(prev_extrema,extrema);
			if verbose then
				print("~~~~~~~~~~~prev_Extrema count", table.getn(prev_extrema));
			end
		
			-- check at lower mask size too, to make sure the lowest possible size is used
			if from==to or (to-from) <=2  and table.getn(prev_extrema)>0 then
				return find_min_index(prev_extrema[table.getn(prev_extrema)]), opt_mask_sizes[table.getn(prev_extrema)];
			end

			return choose_filter_length(from, m, 0);

		elseif from==to or (to-from) <=2 then
			if table.getn(prev_extrema)>0 then
				return find_min_index(prev_extrema[table.getn(prev_extrema)]), opt_mask_sizes[table.getn(prev_extrema)];
			else
				local k;
				table.insert(irregular_extrema, {extr_count, m, extrema});
				max_loc, k =find_least_irregular_extrem(); --FIXME/NB using m - which with current version is ok
				extrema={{max_loc, 1}};
				min_index=find_best_choice_min_area(t, max_loc, false, m);
				table.insert(extrema, {min_index, 0});

				return min_index, m;
			end
		else
			if verbose then
				print("777777777");
			end
			table.insert(irregular_extrema, {extr_count, m, extrema});
			return choose_filter_length(m, to, 0);
		end
	end

	return choose_filter_length(0, size, 0);
end

-- convolution border type = 0:zeroed 1:periodic 2:mirrored 3:constant 
function on_convolution_by_sectors(self, id)
	local sector_size=3;
	local sector_step=3;
	local d_ro=1;

	local img_w, img_h, img_f, img_g = ia_image_get_metrics(app.image);
	--local img_gray=ia_image_convert_gray(app.image, IAT_UINT_8);
	--local img_gray=app.image;

	-- calculate image radius - assume image is with circular shape
	local diameter=math.sqrt(img_w * img_w + img_h * img_h);
	local radius=diameter/2;

	local perimeter=2*(img_w+img_h)-4;
	local img_polar = ia_image_new(radius, perimeter, IAT_UINT_32, false);
	local img_final=ia_image_new(img_w, img_h,  IAT_UINT_32, false);

	on_window_find_center();
	local center_x=app.mass_center_x+1;
	local center_y=app.mass_center_y+1;
	
	local i, j;
	local perimeter_points={};
	for i=0, img_w-1 do table.insert(perimeter_points, {i,0}); end
	for i=1, img_h-1 do table.insert(perimeter_points, {img_w-1,i}); end
	for i=img_w-2,0, -1 do table.insert(perimeter_points, {i,img_h-1}); end
	for i=img_h-2,1, -1 do table.insert(perimeter_points, {0,i}); end

	app.polar_mapping={};
	local polar_line=0;
	local polar_line_sizemap={};

	table.foreachi(perimeter_points, function(i, pair)
		local j,i=pair[1], pair[2];
		-- get line from all parameter points to center_x, center_y	
		app.polar_mapping.current_line={};
		ia_line_draw(center_x, center_y, j, i, "on_image_polar_mapping_line_callback");
		-- cache line size - normally smaller than image radius
		polar_line_sizemap[polar_line]=table.getn(app.polar_mapping.current_line);

		-- draw polar mapped points
		local r;
		for r=1,radius do
			local pair=app.polar_mapping.current_line[r];
			if pair then
				ia_image_set_pixel(img_polar, r-1, polar_line, ia_image_get_pixel(app.image, pair[1], pair[2]));
			end
		end
		polar_line=polar_line+1;
	end);

	assert( polar_line == perimeter, "Calculation error! Invalid perimeter = "..polar_line..". Must be = "..perimeter);

	local img_polar_gray=ia_image_convert_gray(img_polar, IAT_UINT_8);
	ia_image_inverse(img_polar_gray);
	-- find sectors' representing line and calculate LoG
	local start_sector=-sector_step;
	local end_sector=0;

	local contours={};
	for i=0, polar_line-1,sector_step do
		start_sector=start_sector + sector_step;
		end_sector=start_sector + sector_size;

		local mean_intensities={};
		local s, b;
		local sum=0;

		for b=0, polar_line_sizemap[i]-1 do 
			for s=start_sector, end_sector-1 do
				local sector=s;
				if sector > polar_line-1 then
					sector=sector - polar_line;
				end
				if b>polar_line_sizemap[sector]-1 then
					--print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
					sum=sum+ia_image_get_pixel(img_polar_gray, polar_line_sizemap[sector]-1, sector);
				else
					sum=sum+ia_image_get_pixel(img_polar_gray, b, sector);
				end
			end
			if math.mod(b+1, d_ro)==0 then
				table.insert(mean_intensities, sum/sector_step);
				sum=0;
			end
		end

		if i==183 then
			local s="";
			for i,v in mean_intensities do s=s..","..v; end
			print(s);
			--os.exit();
		end
		local min_index, size=locate_contour(mean_intensities, i, id);
		--print(i, min_index, table.getn(mean_intensities));
		if min_index then
			contours[i]=math.floor(min_index);
		end
	end

	-- smooth contour
	local fft_re={};
	local fft_im={};
	local conv_data={};
	for i=0, polar_line-1 do
		if contours[i] then
			table.insert(fft_re, contours[i]);
			table.insert(fft_im, 0);
			table.insert(conv_data, contours[i]);
		end
	end
	
	--do fft contour smoothing by cutting of frequencies beyond the 10th
	fft_re, fft_im=ia_FFT(fft_re, fft_im, 1, 1);
	table.foreachi(fft_re, function(i,_)
		if i>10 and i<table.getn(fft_re)-10 then
			fft_re[i]=0;
			fft_im[i]=0;
		end
	end);
	fft_re, fft_im=ia_FFT(fft_re, fft_im, -1, 1);
	
	-- do gausian contour smoothing with gausian size = center_x/2
	conv_data=ia_convolution(conv_data, generate_filter_gausian(center_x/2), 0, 0, IA_CONVOLUTION_BORDER_TYPE);
	
	-- substitue cleaned out image with background intensities
	local k=0;
	local prev_countour_point=0;
	for i=0, polar_line-1 do
		if contours[i] then
			k=k+1;
			prev_countour_point=k;
		end
		for j=fft_re[prev_countour_point], polar_line_sizemap[i] do
			ia_image_set_pixel(img_polar, j, i, IA_RGB(255,255,255));
		end
	end

	--draw all contours over polar image
	local j=1;
	for i=0, polar_line-1 do
		if contours[i] then
			ia_image_set_pixel(img_polar, contours[i], i, IA_RGB(255,0,0)); --original contour in blue
			-- ia_image_set_pixel(img_polar, math.floor(conv_data[j]), i, IA_RGB(0,255,0)); --gausian smoothed contour in green
			ia_image_set_pixel(img_polar, math.floor(fft_re[j]), i, IA_RGB(0,0,255)); --fft contour in red
			j=j+1;
		end
	end

	
	-- draw clean image according to fft contour
	polar_line=0;
	table.foreachi(perimeter_points, function(i, pair)
		local j,i=pair[1], pair[2];
		-- get line from all parameter points to center_x, center_y	
		app.polar_mapping.current_line={};
		ia_line_draw(center_x, center_y, j, i, "on_image_polar_mapping_line_callback");

		-- draw polar mapped points
		local r;
		for r=1,radius do
			local pair=app.polar_mapping.current_line[r];
			if pair then
				ia_image_set_pixel(img_final, pair[1], pair[2], ia_image_get_pixel(img_polar, r-1, polar_line));
			end
		end
		polar_line=polar_line+1;
	end);

	app.image=img_final;
	if app.mass_center_x and app.mass_center_y then
		local i;
		for i=0,2 do
			ia_image_set_pixel(app.image, app.mass_center_x + i, app.mass_center_y, IA_RGB(0, 0, 255));
		end
		for i=0,2 do
			ia_image_set_pixel(app.image, app.mass_center_x, app.mass_center_y + i, IA_RGB(0, 0, 255));
		end
	end

	ia_image_destroy(img_polar);
	ia_image_destroy(img_polar_gray);
	gia_win_redraw(self);
	print("Convolution finished with result image "..radius.."x"..perimeter);
end

function on_find_min_by_gradient(self, id)
	on_convolution_by_sectors(self, id);
end

function on_find_min_by_partial_bin(self, id)
	on_convolution_by_sectors(self, id);
end

function on_find_min_by_whole_bin(self, id)
	on_convolution_by_sectors(self, id);
end

function on_find_min_by_histogram_bin(self, id)
	on_convolution_by_sectors(self, id);
end

function count_extrema(pixels) -- nadia
	local i=2;
	local extrems=0;
	local increasing=0;
	local prev_increasing=0;
	local inc_loc=0;
	local prev_inc_loc=0;
	local positions={};

	--for i,v in pixels do print("!!!",i,v); end

	while i<=table.getn(pixels)-1 do

		while pixels[i] and pixels[i-1]==pixels[i] do i=i+1; end
		while pixels[i] and pixels[i]<0.001 do i=i+1; end

		if pixels[i] and pixels[i+1] then
			local prev=pixels[i-1];
			local curr=pixels[i];
			local next=pixels[i+1];

			if prev<curr and curr<next then --and curr-prev>1 and next-curr>1 then
				inc_loc=i;
				increasing=1;
			elseif prev>curr and curr>next then --and prev-curr>1 and curr-next>1 then
				increasing=-1;
				inc_loc=i;
			end	
		end

		if prev_increasing ~= 0 and prev_increasing~=increasing then
			local extr_pos=i-1;
			if inc_loc - prev_inc_loc>2 then
				extr_pos=math.floor((inc_loc + prev_inc_loc)/2);
			end
			if prev_increasing == 1 and increasing == -1 then
				table.insert(positions, {extr_pos, 1});
				extrems=extrems+1;
			elseif prev_increasing == -1 and increasing == 1 then
				table.insert(positions, {extr_pos, 0});
				extrems=extrems+1;
			end
		elseif prev_increasing==0 and i==2 and increasing==-1 then --max from very beginning
			table.insert(positions, {1, 1}); 
			extrems=extrems+1;
		elseif prev_increasing==0 and i>2 and increasing==-1 then -- repetitions of same value then decrease
			table.insert(positions, {math.floor(i/2), 1}); 
			extrems=extrems+1;
		elseif prev_increasing ~= 0 and i==table.getn(pixels)-1 and increasing==1 then
			table.insert(positions, {table.getn(pixels), 1}); 
			extrems=extrems+1;
		end
		prev_increasing=increasing;
		prev_inc_loc=inc_loc;
		i=i+1;
	end
	return extrems, positions;
end

function count_extrems(pixels)
	local i=2;
	local extrems=0;
	local increasing=0;
	local prev_increasing=0;
	local positions={};

	while i<table.getn(pixels)-1 do

		while pixels[i] and pixels[i-1]==pixels[i] do i=i+1; end

		if pixels[i] then

			local prev=pixels[i-1];
			local curr=pixels[i];
			local next=pixels[i+1];

			if prev<curr and curr<next then
				increasing=1;
			elseif prev>curr and curr>next then
				increasing=-1;
			end
		
			i=i+1;
		end

		if prev_increasing ~= 0 and prev_increasing~=increasing then
			if prev_increasing == 1 and increasing == -1 then
				table.insert(positions, i);
				extrems=extrems+1;
			end
		end
		prev_increasing=increasing;
	end
	return extrems, positions;
end

function nadia_blur(pixels)
	-- FIXME: not optimised yet
	local m, min_index=locate_contour(pixels, 1);
	local mask=generate_filter_gausian(m);
	return ia_convolution(pixels, mask, 0, 0, IA_CONVOLUTION_BORDER_TYPE), min_index;
end

function find_barrier_position_nadia(pixels)
	local pos;
	local t=nadia_blur(pixels);
	local extrems, positions=count_extrems(t);

	if extrems == 1 then
		pos=positions[1];
	elseif extrems > 1 then
		pos=positions[table.getn(positions)];
	else
		pos=math.floor(table.getn(pixels)/2);
		--pos=positions[ (extrems+1)/2 ];
	end

	return pos;
end

function on_nadia_test(self, id)
	on_convolution_by_sectors(self, id);
end

-- the new version
function find_barrier_position(pixels)
	local function slight_blur(pixels, amount)
		amount=amount or 15;
		local j, i;
		local t={};
		for i=1,table.getn(pixels) do
			table.insert(t, pixels[i]);
		end
		for j=1,amount do
			for i=1,table.getn(pixels) do
				t[i]=((t[i-1] or 0) + (t[i] or 0) + (t[i+1] or 0))/3;
			end
		end
		return t;
	end

	local function count_extrema(pixels)
		local i=2;
		local extrems=0;
		local increasing=0;
		local prev_increasing=0;
		local positions={};

		while i<table.getn(pixels)-1 do

			while pixels[i] and pixels[i-1]==pixels[i] do i=i+1; end

			if pixels[i] then

				local prev=pixels[i-1];
				local curr=pixels[i];
				local next=pixels[i+1];

				if prev<curr and curr<next then
					increasing=1;
				elseif prev>curr and curr>next then
					increasing=-1;
				end
			
				i=i+1;
			end

			if prev_increasing ~= 0 and prev_increasing~=increasing then
				if prev_increasing == 1 and increasing == -1 then
					table.insert(positions, i);
					extrems=extrems+1;
				end
			end
			prev_increasing=increasing;
		end
		return extrems, positions;
	end

	local t=slight_blur(pixels, 15);
	local extrema, positions=count_extrema(t);
	local pos;
	if extrema == 1 then
		pos=positions[1];
	elseif extrema > 1 then
		pos=positions[table.getn(positions)];
	else
		pos=math.floor(1+table.getn(pixels)/2);
	end

	return pos;
end

function on_alek_hack(self)
	local w, h, f, g = ia_image_get_metrics(app.image);
	local img_gray=ia_image_convert_gray(app.image, IAT_UINT_8);
	ia_binarize_otsu(img_gray);

	local pixels={};
	for j=0,w-1 do
		table.insert(pixels, ia_image_get_pixel(img_gray, j, 0));
	end

	print( "Barrier=", find_barrier_position(pixels) );

	local t, i, iter;
	t, nadia_min=nadia_blur(pixels);
	local extrems, positions=count_extrems(t);

	-- find min (e.g. max value)
	local pos, max_val=w/2, -255;
	table.foreachi(t, function(_pos, value)
		if value>max_val then
			pos=_pos;
			max_val=value;
		end
	end);

	if extrems == 1 then
		pos=positions[1];
	elseif extrems == 2 then
		pos=positions[2];
	else
		pos=positions[ (extrems+1)/2 ];
	end

	print("extrems=", extrems, "maximum =", max_val, "at pos =", pos);

	for j=0,w-1 do
		ia_image_set_pixel(img_gray, j, 0, t[j+1]);
	end
	app.image=ia_image_convert_rgb(img_gray);

	ia_image_set_pixel(app.image, nadia_min-1, 0, IA_RGB(255,0,0));

	--table.foreachi(positions, function (_, pos)
		ia_image_set_pixel(app.image, pos-1, 0, IA_RGB(0,0,255));
	--end);

	gia_win_redraw(self);
end

function on_nadia_draw_line(self)
	--local pixels={254,255,240,49,30,24,76,178,244,253,251,247,252,254,255,241,220,81,30,28,42,177,239,255,255,253,0,0,255,253,229,191,211,216,254,255,246,255,254,255,230,125,192,255,255,253,255,234,172,31,193,229,254,255,246,255,254,245,221,78,47,145,255,240,252,254,255,255,253,255,240,249,253,254,222,31,22,227,255,240,255,255,253,241,251,255,253,204,94,21,20,0,45,239,234,255,241,250,241,239,255,255,5,7,6,12,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0}
	--local pixels={255,18,19,31,36,11,13,42,98,194,248,247.5,245,157.75,40.25,8.75,9,29,46,37,31,37,60,81,75,45,21.75,30.5,64.75,208.5,252.5,245,254,236,246.5,218.5,59,13,32.25,17.5,12.5,17,26,19,77.5,164,228.75,255,255,253,254,255,255,255,254,252.5,249,243,246.75,231.75,230,226.25,216.75,202.75,202.5,206.25,196.5,182.75,187.25,168.25,93.75,27.75,11,11.5,13.25,15,15,15,15,15,15,15,15,8.25,19.5,6.75,13.5,41.75,102,147.75,157,182.5,197.25,207.5,217.75,252.75,248.5,246.25,233,78.25,18.25,14.75,15.25,15,15,15.5,15.75,93,181.75,44.5,13.5,20.25,8.25,16.5};
	--local pixels={255,31,31,31,22,35.75,28,44,171,254,247,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254.75,255,255,255,255,254.5,252.5,250.25,253,251.75,248,250.5,253.5,255,212.5,214.25,229.5,242.75,254.25,254,254.5,255,254.5,254.5,255,253,252.5,254.25,249.75,251,253.25,251,255,254,255,255,254,255,127,32,31,31,32,15,16,15,15};
	local pixels={255,255,255,255,255,255,255,255,252,172,108,62,44,38,39,49,38,32,31,31,32,32,32,32,32,32,32,32,32,32,32,31.666666666667,46,31.666666666667,44.333333333333,26.666666666667,27.666666666667,31,28,114.66666666667,178,69.666666666667,46,32.666666666667,32,31.666666666667,31.666666666667,31.666666666667,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31};
	-- local pixels={31,31,31,26,35,28,44,162,255,253,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,249,242,191,122,68,37,33,32,32,32,32,32,32,32,31,30,47,0,28,44,53,58,62,34,119,204,255,0,253,255,255,255,254,255,255,254,255,127,32,31,31,32,15,15,15,15,0};
	-- local pixels={31,255,31,31,31,31,31,31,31,31,31,31,31,31,32,32,29,32,35,132,130,124,248,243,253,250,255,255,240,251,254,241,100,57,28,32,31,19,21,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,0,31,29,32,31,28,82,255,255,255,254,83,31,31,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,30,30,31,31,28,182,255,255,255,255,255,255,255,255,255,255,47,32,32,30,22,0};
	local img_result = ia_image_new(table.getn(pixels), 1, IAT_UINT_8, true);
	table.foreachi(pixels, function(i, v)
		ia_image_set_pixel(img_result, i-1, 0, 255-v);
	end);

	app.image=ia_image_convert_rgb(img_result);
	gia_win_redraw(self);
end

function on_2D_fft(self)
	local w, h, f, g = ia_image_get_metrics(app.image);
	local img_gray=ia_image_convert_gray(app.image, IAT_UINT_8);

	-- do fft for all rows first
	local img_res=ia_image_new( w, h, IAT_UINT_8, true);
	local img_fin=ia_image_new( w, h, IAT_UINT_8, true);

	local img_mapr={};
	local img_mapi={};
	
	for j=0, h-1 do
		img_mapr[j]=img_mapr[j] or {};
		img_mapi[j]=img_mapi[j] or {};

		local re={};
		local im={};
		for i=0, w-1 do
			table.insert(re, ia_image_get_pixel(img_gray, i, j));
			table.insert(im, 0);
		end
		re, im=ia_FFT(re, im, 1, 1);
		for i=0, w-1 do
			img_mapr[j][i]=re[i+1];
			img_mapi[j][i]=im[i+1];
			--ia_image_set_pixel(img_res, i, j, math.sqrt(re[i+1]*re[i+1]+im[i+1]*im[i+1]));
		end
	end

	-- do fft for all cols 
	local max=0;
	local mapr={};
	local mapi={};

	for i=0, w-1 do
		mapr[i]=mapr[i] or {};
		mapi[i]=mapi[i] or {};

		local re={};
		local im={};
		for j=0, h-1 do
			table.insert(re, img_mapr[j][i]);
			table.insert(im, img_mapi[j][i]);
		end
		re, im=ia_FFT(re, im, 1, 1);
		for j=0, h-1 do
			local v=math.sqrt(re[j+1]*re[j+1]+im[j+1]*im[j+1]);
			if v>max then max=v; end

			ia_image_set_pixel(img_res, i, j, v);
			mapr[i][j]=re[j+1];
			mapi[i][j]=im[j+1];
		end
	end

	-- set the logarithm of the spectrum
	local c=255/math.log(1+max);
	for j=0, h-1 do
		for i=0, w-1 do
			local v=c*math.log(1 + ia_image_get_pixel(img_res, i, j));
			if v>255 then v=255; end
			--print(v, math.log(1 + ia_image_get_pixel(img_res, i, j)), ia_image_get_pixel(img_res, i, j));

			if i<w/2 and j<h/2 then --1
				ia_image_set_pixel(img_fin, math.ceil(w/2+i), math.ceil(h/2 + j),v);				
			elseif i<w/2 and j>=h/2 then --4
				ia_image_set_pixel(img_fin, math.ceil(w/2+i), math.ceil(j-h/2),v);
			elseif i>=w/2 and j<h/2 then --2
				ia_image_set_pixel(img_fin, math.ceil(i-w/2), math.ceil(h/2 + j),v);
			elseif i>w/2 and j>=h/2 then --3
				ia_image_set_pixel(img_fin, math.ceil(i-w/2), math.ceil(j-h/2),v);
			end
			
		end
	end
	
	--[[
	-- calculate inverse
	local thresh=max*5/10000000;

	-- do fft for all cols first
	for i=0, w-1 do
		local re={};
		local im={};
		for j=0, h-1 do
			local v=math.sqrt(mapr[i][j]*mapr[i][j]+ mapi[i][j]* mapi[i][j])
			if v>=thresh then
				table.insert(re, mapr[i][j]);
				table.insert(im,  mapi[i][j]);
			else
				table.insert(re, 0);
				table.insert(im,  0);
			end
		end
		re, im=ia_FFT(re, im, -1, 1);
		for j=0, h-1 do
			mapr[i][j]=re[j+1];
			mapi[i][j]=im[j+1];
		end
	end

	for j=0, h-1 do
		local re={};
		local im={};
		for i=0, w-1 do
			table.insert(re, mapr[i][j]);
			table.insert(im, mapi[i][j]);
		end
		re, im=ia_FFT(re, im, -1, 1);
		for i=0, w-1 do
			local v=re[i+1];  --math.sqrt(re[i+1]*re[i+1]+im[i+1]*im[i+1]);
			ia_image_set_pixel(img_fin, i, j, v);
		end
	end	
	]]
	
	app.image=ia_image_convert_rgb(img_fin);
	gia_win_redraw(self);
end

function on_alek_blur(self)
	local w, h, f, g = ia_image_get_metrics(app.image);
	local img_gray=ia_image_convert_gray(app.image, IAT_UINT_8);
	--ia_binarize_otsu(img_gray);

	local pixels={};
	for i=0,w-1 do
		table.insert(pixels, 255 - ia_image_get_pixel(img_gray, i, 0));
	end
	
	local min_index, size=locate_contour(pixels,nil,ID_FIND_MIN_BY_PARTIAL_BIN);
	print("min_index=", min_index, "size=",size, "n=", table.getn(pixels));
	local t=ia_convolution(pixels, generate_filter_gausian(size), 0, 0, IA_CONVOLUTION_BORDER_TYPE); --generate_gausian_parametrized(0.9, table.getn(pixels)),0,0);
	for i,v in t do 
		ia_image_set_pixel(img_gray, i-1,0,255 - v);
	end

	app.image=ia_image_convert_rgb(img_gray);
	ia_image_set_pixel(app.image, min_index, 0, IA_RGB(255,0,0));
	gia_win_redraw(self);
	
end

---------- END NADIA
app=
{
	object         = gia_app_new("ia"),             -- application object
	image          = nil,                           -- image object
	is_binarize    = false,                         -- on/off binarization
	binarize_level = 50,                            -- binarization level in %
	menu_commands  =
	{
		-- File
		[ID_NEW]                 = on_file_new,
		[ID_OPEN]                = on_file_open,
		[ID_SAVE]                = on_file_save,
		---[ID_INFO]                = on_file_info,
		[ID_QUIT]                = on_file_quit,

		-- Transform
		[ID_DT_CITY_BLOCK_S]     = on_dt_sequential,
		[ID_DT_CITY_BLOCK_P]     = on_dt_sequential,
		[ID_DT_CHESSBOARD_S]     = on_dt_sequential,
		[ID_DT_CHESSBOARD_P]     = on_dt_sequential,
		[ID_DT_CHAMFER_3_4_S]    = on_dt_sequential,
		[ID_DT_CHAMFER_3_4_P]    = on_dt_sequential,
		[ID_DT_CHAMFER_5_7_11_S] = on_dt_sequential,
		[ID_DT_CHAMFER_5_7_11_P] = on_dt_sequential,
		[ID_DT_CHAMFER_7X7_S]    = on_dt_sequential,
		[ID_DT_CHAMFER_7X7_P]    = on_dt_sequential,

		-- Image
		[ID_GRAY_SCALE]           = on_image_grayscale,
		[ID_COUNTOURS]           = on_image_contours,
		[ID_FILL]                = on_image_fill,
		[ID_INVERSE]             = on_image_inverse,
		[ID_BINARIZE]            = on_image_binarize,
		[ID_DILATION]            = on_image_dilation,
		[ID_EROSION]             = on_image_erosion,
		[ID_OPENING]             = on_image_opening,
		[ID_CLOSING]             = on_image_closing,

		-- Window
		[ID_BINARIZATION]        = on_window_binarization,
		[ID_TREE]                = on_window_tree,
		[ID_FIND_PIC]            = on_window_find_pic,
		[ID_FIND_CENTER]         = on_window_find_center,
		[ID_MASSIVE_CONTOURS]    = on_window_massive_contours,
		[ID_CUT_MASSIVE_CONTOURS]= on_window_cut_massive_contours,

		-- Heuristics
		--[ID_HEURISTIC_1]         = on_heuristic_1,
		--[ID_HEURISTIC_2]         = on_heuristic_2,
		--[ID_HEURISTIC_3]         = on_heuristic_3,

		-- nadia
		[ID_CONVOLUTION_BY_SECTORS]	= on_convolution_by_sectors,
		[ID_ALEK_HACK]			= on_alek_hack,
		[ID_ALEK_BLUR]			= on_alek_blur,
		[ID_NADIA_TEST]			= on_nadia_test,
		[ID_NADIA_DRAW_LINE]		= on_nadia_draw_line,
		[ID_FIND_MIN_BY_GRADIENT]	= on_find_min_by_gradient,
		[ID_FIND_MIN_BY_PARTIAL_BIN]	= on_find_min_by_partial_bin,
		[ID_FIND_MIN_BY_WHOLE_BIN]	= on_find_min_by_whole_bin,
		[ID_FIND_MIN_BY_HIST_BIN]	= on_find_min_by_histogram_bin,
		[ID_2D_FFT]			= on_2D_fft,
		
	},
	dialog_item_commands=
	{
		[IDD_BINARIZE_SLIDEBAR]  = on_window_binarization_scroll,
		[IDD_BINARIZE_CHECKBOX]  = on_window_binarization_check,
	},
};

function create_menu()
	local menu_file, menu_dt, menu_image, menu_window, menu_heuristics, menu_nadia;

	menu_file = gia_menu_popup_new("&File");
	gia_menu_popup_add(menu_file, "&New");
	gia_menu_popup_add(menu_file, "&Open");
	gia_menu_popup_add(menu_file, "&Save");
	--gia_menu_popup_add(menu_file, "&Info");
	gia_menu_popup_add(menu_file, "&Quit");

	menu_dt  = gia_menu_popup_new("&Distance Transform");
	gia_menu_popup_add(menu_dt, "&0. City Block seq.");
	gia_menu_popup_add(menu_dt, "&1. City Block par.");
	gia_menu_popup_add(menu_dt, "&2. Chessboard seq.");
	gia_menu_popup_add(menu_dt, "&3. Chessboard par.");
	gia_menu_popup_add(menu_dt, "&4. Chamfer 3-4 seq.");
	gia_menu_popup_add(menu_dt, "&5. Chamfer 3-4 par.");
	gia_menu_popup_add(menu_dt, "&6. Chamfer 5-7-11 seq.");
	gia_menu_popup_add(menu_dt, "&7. Chamfer 5-7-11 par.");
	gia_menu_popup_add(menu_dt, "&8. Chamfer 7x7 12-17-27-38-43 seq.");
	gia_menu_popup_add(menu_dt, "&9. Chamfer 7x7 12-17-27-38-43 par.");

	menu_image  = gia_menu_popup_new("&Image");
	gia_menu_popup_add(menu_image, "&Grayscale");
	gia_menu_popup_add(menu_image, "&Contours");
	gia_menu_popup_add(menu_image, "&Fill");
	gia_menu_popup_add(menu_image, "&Inverse");
	gia_menu_popup_add(menu_image, "&Binarize");
	gia_menu_popup_add(menu_image, "&Dilation");
	gia_menu_popup_add(menu_image, "&Erosion");
	gia_menu_popup_add(menu_image, "&Opening");
	gia_menu_popup_add(menu_image, "C&losing");

	menu_window  = gia_menu_popup_new("&Algorithms");
	gia_menu_popup_add(menu_window, "&Binarization");
	gia_menu_popup_add(menu_window, "&Tree");
	gia_menu_popup_add(menu_window, "Find Func. &Pic");
	gia_menu_popup_add(menu_window, "Find &Center");
	gia_menu_popup_add(menu_window, "&Massive Contours");
	gia_menu_popup_add(menu_window, "&Cut Massive Contours");

	--menu_heuristics  = gia_menu_popup_new("&Heuristics");
	--gia_menu_popup_add(menu_heuristics, "&Heuristic1");
	--gia_menu_popup_add(menu_heuristics, "&Heuristic2");
	--gia_menu_popup_add(menu_heuristics, "&Heuristic3");

	menu_nadia = gia_menu_popup_new("&Nadia");
	gia_menu_popup_add(menu_nadia, "&Find MIN with exponential distribution");
	gia_menu_popup_add(menu_nadia, "&HACK");
	gia_menu_popup_add(menu_nadia, "&BLUR");
	gia_menu_popup_add(menu_nadia, "&Convolutino with min max");
	gia_menu_popup_add(menu_nadia, "&Draw Line");
	gia_menu_popup_add(menu_nadia, "&Find MIN by max gradient");
	gia_menu_popup_add(menu_nadia, "&Find MIN by partial binarization");
	gia_menu_popup_add(menu_nadia, "&Find MIN by whole binarization");
	gia_menu_popup_add(menu_nadia, "&Find MIN by 2nd derivative test");
	gia_menu_popup_add(menu_nadia, "&2D FFT");
	
	return gia_menu_bar(menu_file, menu_dt, menu_image, menu_window); --, menu_heuristics);
end

function gia_message(self, msg, ...)
	if msg == "on_destroy" then
		on_destroy(self);
	elseif msg == "on_close" then
		on_close(self);
	elseif msg == "on_command" then
		app.menu_commands[arg[1]+1](self, arg[1]+1);
	elseif msg == "on_dialog_command" then
		local sender=arg[1];
		local dlg_item_id=gia_win_get_control_id(sender);
		local cmd=app.dialog_item_commands[dlg_item_id];
		if cmd then
			cmd(self, sender);
		end
	elseif msg == "on_paint" then
		on_paint(self);
	elseif msg == "on_mouseclick" then
		on_mouseclick(self, unpack(arg));
	elseif msg == "on_mousemove" then
		on_mousemove(self, unpack(arg));
	end
end

app.main_window=gia_app_window_new(app.object, 1, 1, 640, 480, 0, create_menu(), 0, nil);
gia_win_set_title(app.main_window, "Image Analyzing Laboratory");
gia_app_set_main_window(app.object, app.main_window);
gia_win_set_visible(app.main_window, true);
on_file_new(app.main_window);

--on_file_open(app.main_window, nil, "C:/cvs/lrun/test/manual/modules/gui/ia/logo/bg1005y.jpg");
--on_file_open(app.main_window, nil, "C:/cvs/lrun/test/manual/modules/gui/ia/test.img");
-- on_file_open(app.main_window, nil, "C:/home/alek/ia/data/test3.img");

gia_app_run(app.object);

