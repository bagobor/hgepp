/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: resources management
 */
#include "hge_impl.h"

#define NOCRYPT
//#define NOUNCRYPT
#include <unzip.h>

namespace hge
{

bool HGE_CALL HGE_Impl::Resource_AttachPack(const char *filename, const char *password)
{
	char *szName;
	CResourceList *resItem = m_res_list;
	unzFile zip;

	szName = Resource_MakePath(filename);
	strupr(szName);

	while (resItem)
	{
		if (!strcmp(szName, resItem->filename))
			return false;
		resItem = resItem->next;
	}

	zip = unzOpen(szName);
	if (!zip)
		return false;
	unzClose(zip);

	resItem = new CResourceList;
	strcpy(resItem->filename, szName);
	if (password)
		strcpy(resItem->password, password);
	else
		resItem->password[0] = 0;
	resItem->next = m_res_list;
	m_res_list = resItem;

	return true;
}

void HGE_CALL HGE_Impl::Resource_RemovePack(const char *filename)
{
	char *szName;
	CResourceList *resItem = m_res_list, *resPrev = 0;

	szName = Resource_MakePath(filename);
	strupr(szName);

	while (resItem)
	{
		if (!strcmp(szName, resItem->filename))
		{
			if (resPrev)
				resPrev->next = resItem->next;
			else
				m_res_list = resItem->next;
			delete resItem;
			break;
		}

		resPrev = resItem;
		resItem = resItem->next;
	}
}

void HGE_CALL HGE_Impl::Resource_RemoveAllPacks()
{
	CResourceList *resItem = m_res_list, *resNextItem;

	while (resItem)
	{
		resNextItem = resItem->next;
		delete resItem;
		resItem = resNextItem;
	}

	m_res_list = 0;
}

void* HGE_CALL HGE_Impl::Resource_Load(const char *filename, uint32_t *size)
{
	static char *res_err = "Can't load resource: %s";

	CResourceList *resItem = m_res_list;
	char szName[_MAX_PATH];
	char szZipName[_MAX_PATH];
	unzFile zip;
	unz_file_info file_info;
	int done, i;
	void *ptr;
	HANDLE hF;

	if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':')
		goto _fromfile; // skip absolute paths

	// Load from pack

	strcpy(szName, filename);
	strupr(szName);
	for (i = 0; szName[i]; i++)
	{
		if (szName[i] == '/')
			szName[i] = '\\';
	}

	while (resItem)
	{
		zip = unzOpen(resItem->filename);
		done = unzGoToFirstFile(zip);
		while (done == UNZ_OK)
		{
			unzGetCurrentFileInfo(zip, &file_info, szZipName, sizeof(szZipName), NULL, 0, NULL, 0);
			strupr(szZipName);
			for (i = 0; szZipName[i]; i++)
			{
				if (szZipName[i] == '/')
					szZipName[i] = '\\';
			}
			if (!strcmp(szName, szZipName))
			{
				if (unzOpenCurrentFilePassword(zip, resItem->password[0] ? resItem->password : 0)
						!= UNZ_OK)
				{
					unzClose(zip);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}

				ptr = malloc(file_info.uncompressed_size);
				if (!ptr)
				{
					unzCloseCurrentFile(zip);
					unzClose(zip);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}

				if (unzReadCurrentFile(zip, ptr, file_info.uncompressed_size) < 0)
				{
					unzCloseCurrentFile(zip);
					unzClose(zip);
					free(ptr);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}
				unzCloseCurrentFile(zip);
				unzClose(zip);
				if (size)
					*size = file_info.uncompressed_size;
				return ptr;
			}

			done = unzGoToNextFile(zip);
		}

		unzClose(zip);
		resItem = resItem->next;
	}

	// Load from file
	_fromfile:

	hF = CreateFile(Resource_MakePath(filename), GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hF == INVALID_HANDLE_VALUE)
	{
		sprintf(szName, res_err, filename);
		_PostError(szName);
		return 0;
	}
	file_info.uncompressed_size = GetFileSize(hF, NULL);
	ptr = malloc(file_info.uncompressed_size);
	if (!ptr)
	{
		CloseHandle(hF);
		sprintf(szName, res_err, filename);
		_PostError(szName);
		return 0;
	}
	if (ReadFile(hF, ptr, file_info.uncompressed_size, &file_info.uncompressed_size, NULL ) == 0)
	{
		CloseHandle(hF);
		free(ptr);
		sprintf(szName, res_err, filename);
		_PostError(szName);
		return 0;
	}

	CloseHandle(hF);
	if (size)
		*size = file_info.uncompressed_size;
	return ptr;
}

void HGE_CALL HGE_Impl::Resource_Free(void *res)
{
	if (res)
		free(res);
}

char* HGE_CALL HGE_Impl::Resource_MakePath(const char *filename)
{
	int i;

	if (!filename)
		strcpy(m_tmp_filename, m_app_path);
	else if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':')
		strcpy(m_tmp_filename, filename);
	else
	{
		strcpy(m_tmp_filename, m_app_path);
		if (filename)
			strcat(m_tmp_filename, filename);
	}

	for (i = 0; m_tmp_filename[i]; i++)
	{
		if (m_tmp_filename[i] == '/')
			m_tmp_filename[i] = '\\';
	}
	return m_tmp_filename;
}

char* HGE_CALL HGE_Impl::Resource_EnumFiles(const char *wildcard)
{
	if (wildcard)
	{
		if (m_hsearch)
		{
			FindClose(m_hsearch);
			m_hsearch = 0;
		}
		m_hsearch = FindFirstFile(Resource_MakePath(wildcard), &m_search_data);
		if (m_hsearch == INVALID_HANDLE_VALUE)
		{
			m_hsearch = 0;
			return 0;
		}

		if (!(m_search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return m_search_data.cFileName;
		else
			return Resource_EnumFiles();
	}
	else
	{
		if (!m_hsearch)
			return 0;
		for (;;)
		{
			if (!FindNextFile(m_hsearch, &m_search_data))
			{
				FindClose(m_hsearch);
				m_hsearch = 0;
				return 0;
			}
			if (!(m_search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				return m_search_data.cFileName;
		}
	}
}

char* HGE_CALL HGE_Impl::Resource_EnumFolders(const char *wildcard)
{
	if (wildcard)
	{
		if (m_hsearch)
		{
			FindClose(m_hsearch);
			m_hsearch = 0;
		}
		m_hsearch = FindFirstFile(Resource_MakePath(wildcard), &m_search_data);
		if (m_hsearch == INVALID_HANDLE_VALUE)
		{
			m_hsearch = 0;
			return 0;
		}

		if ((m_search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(
				m_search_data.cFileName, ".") && strcmp(m_search_data.cFileName, ".."))
			return m_search_data.cFileName;
		else
			return Resource_EnumFolders();
	}
	else
	{
		if (!m_hsearch)
			return 0;
		for (;;)
		{
			if (!FindNextFile(m_hsearch, &m_search_data))
			{
				FindClose(m_hsearch);
				m_hsearch = 0;
				return 0;
			}
			if ((m_search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(
					m_search_data.cFileName, ".") && strcmp(m_search_data.cFileName, ".."))
				return m_search_data.cFileName;
		}
	}
}

} // namespace hge
