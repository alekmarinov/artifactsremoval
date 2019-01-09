/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_common.c                                        */
/* Description:   Image Analyser common implementations              */
/*                                                                   */
/*********************************************************************/

#include <ia/ia.h>
#include <stdio.h>

const ia_value_t ia_0 = {0};

static const ia_string_t ia_format_names[] = 
{
	"IAT_UINT_8", "IAT_UINT_16", "IAT_UINT_24", "IAT_UINT_32", 
	"IAT_INT_8",  "IAT_INT_16",  "IAT_INT_24",  "IAT_INT_32",
	"IAT_FLOAT",  "IAT_DOUBLE",  "IAT_BOOL"
};

ia_uint16_t ia_format_size(ia_format_t format)
{
	switch (format)
	{
		case IAT_BOOL:  
			return 1;
		case IAT_INT_8:  case IAT_UINT_8:  
			return sizeof(ia_uint8_t) << 3;
		case IAT_INT_16:  case IAT_UINT_16: 
			return sizeof(ia_uint16_t) << 3;
		case IAT_INT_24:  case IAT_UINT_24: 
			return sizeof(ia_uint24_t) << 3;
		case IAT_INT_32: case IAT_UINT_32: 
			return sizeof(ia_uint32_t) << 3;
		case IAT_FLOAT:   
			return sizeof(ia_float_t) << 3;
		case IAT_DOUBLE:  
			return sizeof(ia_double_t) << 3;
	default:
		ASSERT(0), "ia_format_size(%d) -> invalid format!\n", format);
	}
	return 0;
}

const ia_string_t ia_format_name(ia_format_t format)
{
	return ia_format_names[format];
}

ia_bool_t ia_format_signed(ia_format_t format)
{
	switch (format)
	{
	case IAT_INT_8:
	case IAT_INT_16:
	case IAT_INT_24:
	case IAT_INT_32:
		return 1;
	}
	return 0;
}

void ia_format_min_max(ia_format_t format, ia_int32_t* min, ia_uint32_t* max)
{
	switch (format)
	{
		case IAT_BOOL:
			*min=0; 
			*max=1;
			break;
		case IAT_INT_8:  
			*min=-128;
			*max=+127;
			break;
		case IAT_UINT_8:  
			*min=0; 
			*max=255;
			break;
		case IAT_INT_16:  
			*min=-32768; 
			*max=32767;
			break;
		case IAT_UINT_16: 
			*min=0; 
			*max=0xFFFF;
			break;
		case IAT_INT_24:
			*min=0x800000;
			*max=0x7FFFFF;
			break;
		case IAT_UINT_24: 
			*min=0;
			*max=0xFFFFFF;
			break;
		case IAT_INT_32: 
			*min=0x80000000;
			*max=IA_MAX_INT;
			break;
		case IAT_UINT_32: 
			*min=0;
			*max=IA_MAX_UINT;
			break;
	default:
		ASSERT(0), "%s is invalid or not supported format by ia_format_min_max!\n", ia_format_names[format]);
	}
}

ia_double_t ia_value_todouble(ia_format_t format, ia_value_t value)
{
	switch (format)
	{
		case IAT_BOOL:
			return (ia_double_t)((value.bl)?1.0f:0.0f);
		case IAT_FLOAT:
			return (ia_double_t)(value.f);
		case IAT_DOUBLE:
			return value.d;
		case IAT_INT_8:
		case IAT_UINT_8:
			return (ia_double_t)(value.bt);
		case IAT_INT_16:
		case IAT_UINT_16:
			return (ia_double_t)(value.s);
		case IAT_INT_32:
		case IAT_UINT_32:
			return (ia_double_t)(value.n);
	}
	ASSERT(0), "%s is invalid or not supported format by ia_value_todouble!\n", ia_format_names[format]);
	return 0.0f;
}

ia_value_t ia_double_tovalue(ia_format_t format, ia_double_t number)
{
	ia_value_t value;
	switch (format)
	{
		case IAT_BOOL:
			if (number<0)number=-number;
			value.bl=(number>0.0000001);
		break;
		case IAT_FLOAT:
			value.f=(ia_float_t)number;
		break;
		case IAT_DOUBLE:
			value.d=number;
		break;
		case IAT_INT_8:
		case IAT_UINT_8:
			value.bt=(ia_int8_t)number;
		break;
		case IAT_INT_16:
		case IAT_UINT_16:
			value.s=(ia_int16_t)number;
		break;
		case IAT_INT_32:
		case IAT_UINT_32:
			value.n=(ia_int32_t)number;
		break;
	}
	ASSERT(0), "%s is invalid or not supported format by ia_value_todouble!\n", ia_format_names[format]);
	return value;
}
