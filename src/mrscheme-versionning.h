/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */ /*
 * mrscheme-desktop
 * Copyright (C) Lancelot SIX 2012 <lancelot.six@lip6.fr>
 * 
 * mrscheme-desktop is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mrscheme-desktop is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#ifndef _MRSCHEME_VERSIONNING_H
#define _MRSCHEME_VERSIONNING_H

	#define MR_SCHEME_LOCAL_VERSION_FILE_URI  MRSCHEME_LOCAL_BASE "VERSION"
	#define MR_SCHEME_REMOTE_VERSION_FILE_URI MRSCHEME_WEB_BASE "VERSION"

	enum version_choice_t {
		MR_SCHEME_VERSION_NOT_FOUND,
		MR_SCHEME_VERSION_LOCAL,
		MR_SCHEME_VERSION_REMOTE
	};

	/*
	 * Selects which version of MrScheme should be used.
	 *
	 * The newer version of both remote and local should be used. If both
	 * are same version number, then local version should be prefered.
	 */
	enum version_choice_t select_adequate_version();

#endif
