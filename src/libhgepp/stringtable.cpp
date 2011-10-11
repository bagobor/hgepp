/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeStringTable helper class implementation
 */

#include <hgepp/stringtable.h>

#include <ctype.h>

namespace hge
{

const char STRHEADERTAG[] = "[HGESTRINGTABLE]";
const char STRFORMATERROR[] = "String table %s has incorrect format.";

HGE * g_hgestringtab_hge = 0;

hgeStringTable::hgeStringTable(const std::string & filename)
{
	int i;
	uint32_t size;
	char *desc, *pdesc;
	NamedString *str;
	char str_name[MAXSTRNAMELENGTH];
	char *str_value, *pvalue;

	g_hgestringtab_hge = hgeCreate(HGE_VERSION);
	strings = 0;

	// load string table file
	bytes_t data = g_hgestringtab_hge->Resource_Load(filename.c_str(), &size);
	if (!data)
		return;

	desc = new char[size + 1];
	memcpy(desc, data.get(), size);
	desc[size] = 0;
	//g_hgestringtab_hge->Resource_Free(data);

	// check header
	if (memcmp(desc, STRHEADERTAG, sizeof(STRHEADERTAG) - 1))
	{
		g_hgestringtab_hge->System_Log(STRFORMATERROR, filename.c_str());
		delete[] desc;
		return;
	}

	pdesc = desc + sizeof(STRHEADERTAG);
	str_value = new char[8192];

	for (;;)
	{
		// skip whitespaces
		while (isspace(*pdesc))
			pdesc++;
		if (!*pdesc)
			break;

		// skip comments
		if (*pdesc == ';')
		{
			while (*pdesc && *pdesc != '\n')
				pdesc++;
			pdesc++;
			continue;
		}

		// get string name -> str_name
		i = 0;
		while (pdesc[i] && pdesc[i] != '=' && !isspace(pdesc[i]) && i < MAXSTRNAMELENGTH)
		{
			str_name[i] = pdesc[i];
			i++;
		}
		str_name[i] = 0;
		pdesc += i;

		// skip string name overflow characters
		while (*pdesc && *pdesc != '=' && !isspace(*pdesc))
			pdesc++;
		if (!*pdesc)
			break;

		// skip whitespaces to '='
		while (isspace(*pdesc))
			pdesc++;
		if (*pdesc != '=')
		{
			g_hgestringtab_hge->System_Log(STRFORMATERROR, filename);
			break;
		}
		pdesc++;

		// skip whitespaces to '"'
		while (isspace(*pdesc))
			pdesc++;
		if (*pdesc != '"')
		{
			g_hgestringtab_hge->System_Log(STRFORMATERROR, filename);
			break;
		}
		pdesc++;

		// parse string value till the closing '"' -> str_value
		// consider: \", \n, \\, LF, CR, whitespaces at line begin/end
		pvalue = str_value;

		while (*pdesc && *pdesc != '"')
		{
			if (*pdesc == '\n' || *pdesc == '\r')
			{
				while (isspace(*pdesc))
					pdesc++;

				pvalue--;
				while (pvalue >= str_value && isspace(*pvalue))
					pvalue--;
				pvalue++;
				*pvalue = ' ';
				pvalue++;

				continue;
			}

			if (*pdesc == '\\')
			{
				pdesc++;
				if (!*pdesc)
					continue;
				if (*pdesc == 'n')
					*pvalue = '\n';
				else
					*pvalue = *pdesc;
				pvalue++;
				pdesc++;
				continue;
			}

			*pvalue = *pdesc;
			pvalue++;
			pdesc++;
		}

		*pvalue = 0;

		// add the parsed string to the list
		str = new NamedString;
		strcpy(str->name, str_name);
		str->string = new char[strlen(str_value) + 1];
		strcpy(str->string, str_value);
		str->next = strings;
		strings = str;

		if (!*pdesc)
			break;
		pdesc++;
	}

	delete[] str_value;
	delete[] desc;
}

hgeStringTable::~hgeStringTable()
{
	NamedString *str, *strnext;

	str = strings;
	while (str)
	{
		strnext = str->next;
		delete[] str->string;
		delete str;
		str = strnext;
	}

	g_hgestringtab_hge->Release();
}

char *hgeStringTable::GetString(const char *name)
{
	NamedString *str = strings;

	while (str)
	{
		if (!strcmp(name, str->name))
			return str->string;
		str = str->next;
	}

	return 0;
}

HGE * hgeStringTable::get_hge()
{
	return g_hgestringtab_hge;
}

} // namespace hge
