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

#include "gtk-mr-scheme.h"
#include "mrscheme-versionning.h"
#include <ctype.h>
#include <curl/curl.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 *                                                                            *
 *                      Helper function internal to the                       *
 *                         mrscheme-versionning module                        *
 *                                                                            *
 ******************************************************************************/

/*
 * Remove all non printable character at the end of str. Replace them with
 * '\0'
 */
void rtrim(char* str) {
	int i;
	// Goto the end of the string
	for (i=0; str[i]!='\0'; i++);
	// Remove all non alphanum chars starting from the end
	for (; !isalnum(str[i]); str[i--]='\0');
}

/*
 * libcurl callback
 */
struct dl_info_t {
	int   alread_downloaded;
	char* data;
};
size_t manage_dl(char *buffer, size_t size, size_t nmemb, void *userp) {
	struct dl_info_t* dl_info = userp;

	(*dl_info).data = realloc((*dl_info).data, (*dl_info).alread_downloaded + size * nmemb);
	memcpy((*dl_info).data + (*dl_info).alread_downloaded, buffer, nmemb*size);
	(*dl_info).alread_downloaded += (nmemb*size);
	
	return nmemb*size;
}

/*
 * Return a string representing the remote version of MrScheme.
 *
 * The string MUST BE FREED by the caller.
 *
 * If the version cannot be guessed, NULL is returned
 */
char* get_remote_version() {
	CURL*            curl;
	CURLcode         res;
	char*            ret;
	struct dl_info_t rem;

	rem.alread_downloaded = 0;
	rem.data = NULL;

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL,            MR_SCHEME_REMOTE_VERSION_FILE_URI);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  manage_dl);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &rem);

		res = curl_easy_perform(curl);
		if (res==CURLE_OK) {
			ret = malloc(rem.alread_downloaded+1);
			memcpy(ret, rem.data, rem.alread_downloaded);
			ret[rem.alread_downloaded] = '\0';
			rtrim(ret);

			free (rem.data);
		} else {
			#ifdef DEBUG
			fprintf(stderr, _("Underlying libcurl error : %d\n"), res);
			#endif
			ret = NULL;
		}

		curl_easy_cleanup (curl);
	} else {
		ret = NULL;
	}
	return ret;
}

/*
 * Return a string representing the local version of MrScheme.
 *
 * The string MUST BE FREED by the caller.
 *
 * If the version cannot be guessed, NULL is returned
 */
char* get_local_version(){
	char* local_path = MR_SCHEME_LOCAL_VERSION_FILE_URI;
	char* ret;
	long  file_size;
	local_path += 7; // Ignore the file:// which has a length of 7

	FILE* loc_file;

	loc_file = fopen(local_path, "r");
	if (loc_file) {
		// retreive size of the file
		fseek(loc_file, 0L, SEEK_END);
		file_size = ftell(loc_file);
		rewind(loc_file);

		ret = malloc( (file_size+1) );

		if ( fread(ret, sizeof(char), file_size, loc_file) != file_size) {
			free(ret);
		} else {
			// Ensure a NULL terminated string
			ret[file_size] = '\0';
			rtrim(ret);
		}
		fclose(loc_file);
	} else {
		#ifdef DEBUG
		fprintf(stderr, _("Unable to guess the version of local MrScheme at %s."
		                " Please check your installation\n"), local_path);
		#endif
		ret = NULL;
	}
	return ret;
}

/******************************************************************************
 *                                                                            *
 *                     Implementats the public API of the                     *
 *                        mrscheme-versionning module                         *
 *                                                                            *
 ******************************************************************************/
enum version_choice_t use_remote_version() {
	char* remote_version = get_remote_version();
	char* local_version  = get_local_version();
	int ret = 0;

	#ifdef DEBUG
	printf(_("Remote version : %s\nLocal version : %s\n"),
	       remote_version,
	       local_version);
	#endif

	if (remote_version == NULL) {
		// No internet. Only local version availalble
		ret = MR_SCHEME_LOCAL;
	} else {
		if (strcmp(local_version, remote_version)<0) {
			ret = MR_SCHEME_REMOTE;
		} else {
			ret = MR_SCHEME_LOCAL;
		}
	}

	#ifdef DEBUG
	printf(_("Using %s version of MrScheme\n"),
	       (ret==MR_SCHEME_LOCAL?_("local"):_("remote")));
	#endif

	// Cleanup allocated memory
	if (remote_version) free (remote_version);
	if (local_version)  free (local_version);
	
	return ret;
}


