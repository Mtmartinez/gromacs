/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright 1991- The GROMACS Authors
 * and the project initiators Erik Lindahl, Berk Hess and David van der Spoel.
 * Consult the AUTHORS/COPYING files and https://www.gromacs.org for details.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * https://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at https://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out https://www.gromacs.org.
 */
/*! \libinternal \file
 * \brief
 * Provides OS-specific directory-name separator
 *
 * \inlibraryapi
 * \ingroup module_utility
 */
#ifndef GMX_UTILITY_DIR_SEPARATOR_H
#define GMX_UTILITY_DIR_SEPARATOR_H

#include "config.h"

/*! \def DIR_SEPARATOR
 * \brief
 * Directory separator on this OS.
 *
 * Native Windows uses backslash path separators (but accepts also slashes).
 * Cygwin and most other systems use slash.
 *
 * \todo Get rid of this (Issue #950). It is not necessary for
 * constructing paths on the systems that it currently supports, and
 * is not reliable in parsing input paths either, since Windows needs
 * to accept both instead of only DIR_SEPARATOR. At the very least, we
 * should refactor the clients of this header so that they operate
 * upon path objects rather than raw path strings.
 */
#if GMX_NATIVE_WINDOWS
#    define DIR_SEPARATOR '\\'
#else
#    define DIR_SEPARATOR '/'
#endif

#endif
