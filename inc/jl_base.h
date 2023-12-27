/*
 * Copyright (c) 2014-2023 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */

#ifndef __JL_BASE_H__
#define __JL_BASE_H__

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
  #error "Don't support big endian in current version"
#endif

#include "jl_config.h"
#include "jl_types.h"
#include "jl_debug.h"
#include "jl_error.h"
#include "portable/jl_portable.h"
#include "jl_switch.h"
#include "driver/jl_reg_io.h"

#define JL_ARRAY_SIZE(x)		(sizeof(x)/sizeof((x)[0]))

#endif /* __JL_BASE_H__ */
