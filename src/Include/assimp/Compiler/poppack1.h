
/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: poppack1.h
 * Last Change: 
 */
 

// ===============================================================================
// May be included multiple times - resets structure packing to the defaults 
// for all supported compilers. Reverts the changes made by #include <pushpack1.h> 
//
// Currently this works on the following compilers:
// MSVC 7,8,9
// GCC
// BORLAND (complains about 'pack state changed but not reverted', but works)
// ===============================================================================

#ifndef AI_PUSHPACK_IS_DEFINED
#	error pushpack1.h must be included after poppack1.h
#endif

// reset packing to the original value
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop )
#endif
#undef PACK_STRUCT

#undef AI_PUSHPACK_IS_DEFINED