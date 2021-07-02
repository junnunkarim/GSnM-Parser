#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

FILE *CurrentFile;
FILE *NewFile;
bool alt_end = false;

bool ReadFile(char *file_name);
char *ExtractText(const char *const string, const char *const start, const char *const end);
void JumpAltEnd(char *currentLine_f, char *file_name_f);
void JumpLine(char *currentLine_f);
void PrintCharacterName(char *currentLine_f);
void LineCleanup(char *currentLine_f);

void main(int argc, char **argv)
{
	if(argc == 2)
	{
		if(!ReadFile(argv[1]))
		{
			printf("Invalid File\n");
			//free(CurrentFile);
			return;
		}

		printf("\n");
	}
	else if(argc < 2)
		printf("Error! No arguments!\n"
				"Usage: ./<g-parser executable> filename.txt\n");
	else
		printf("Error! Too many arguments!\n"
				"Usage: ./<g-parser executable> filename.txt\n");

	return;
}

bool ReadFile(char *file_name)
{
	//printf("%s\n", file_name); //for debugging

  if( (CurrentFile=fopen(file_name,"r")) == NULL ) //check to see if fopen can retrieve the file
  {
    exit(1);
  }
  
	//printf("%s", file_name); //for debugging
	
	//reads the file if it is successfully retrieved
	char *new_file_name = ExtractText(file_name, "/", ".");
  char *currentLine = NULL;
  size_t length = 0;
  ssize_t read;
  
  strcat(new_file_name, ".html");
  
 	if( (NewFile = fopen(new_file_name,"w")) == NULL )
	{
		exit(1);
	}

  while( (read = getline(&currentLine, &length, CurrentFile)) != EOF )
  {
		//printf("%s\n", currentLine); //for debugging

		JumpAltEnd(currentLine, file_name);
			
		//if the line contains choices than creates jump points
		if(strstr(currentLine, "*select"))
			JumpLine(currentLine);
		
		//prints the name of the character who is currently speaking
		if(strstr(currentLine, "nm t="))
			PrintCharacterName(currentLine);
		
		//prints the line and after that loops to get a new line
		if(strstr(currentLine, "[np]") || strstr(currentLine, "[wvl]"))
			LineCleanup(currentLine);
  }
  
  fclose(NewFile);
  fclose(CurrentFile);
  //free(currentLine);

  return true;
}

char *ExtractText(const char *const string, const char *const start, const char *const end)
{
    char  *head = "";
    char  *tail = "";
    char  *result = "";
    size_t length;

    if ((string == NULL) || (start == NULL) || (end == NULL))
        return NULL;

    length = strlen(start);
    head   = strstr(string, start);
    if (head == NULL)
        return NULL;

    head += length;
    tail  = strstr(head, end);
    if (tail == NULL)
        return tail;

    length = tail - head;
    result = malloc(1 + length);
    if (result == NULL)
        return NULL;

    result[length] = '\0';
    memcpy(result, head, length);

		//printf("%s\n", result); //prints unformatted string(for debugging)

    return result;
}

void JumpAltEnd(char *currentLine_f, char *file_name_f)
{	
		if(strstr(file_name_f, "gt01.ks") && (!alt_end))
		{
			fprintf(NewFile, "<p id=\"gt01\">------------------------------------------------------------</p>\n");
			alt_end = true;
		}
		else if(strstr(file_name_f, "gk01.ks") && (!alt_end))
		{
			fprintf(NewFile, "<p id=\"gk01\">------------------------------------------------------------</p>\n");
			alt_end = true;
		}
		else if(strstr(file_name_f, "gm01.ks") && (!alt_end))
		{
			fprintf(NewFile, "<p id=\"gm01\">------------------------------------------------------------</p>\n");
			alt_end = true;
		}
		
		if(strstr(currentLine_f, "@jump storage=\"gt01.ks\""))
		{
			fprintf(NewFile, "<a href=\"gt01.html#gt01\">Tsubaki End</a><br><br>\n");
		}
		else if(strstr(currentLine_f, "@jump storage=\"gk01.ks\""))
		{
			fprintf(NewFile, "<a href=\"gk01.html#gk01\">Kanon End</a><br><br>\n");
		}
		else if(strstr(currentLine_f, "storage=\"gm01.ks\""))
		{
			fprintf(NewFile, "<a href=\"gm01.html#gm01\">Mizuha End</a><br><br>\n");
		}
}

void JumpLine(char *currentLine_f)
{
	//goes here if the line doesn't contain "exlink txt" and has "target" in it
	//an example would be this - |@jump target="*select0" cond="f.flag_tubaki>0"|
	if( strstr(currentLine_f, "target=\"") && !strstr(currentLine_f, "exlink txt=") )
	{
		//goes here if there is condition on the line
		if(strstr(currentLine_f, "cond="))
		{
			//creates a jump link with href
			fprintf(NewFile, "<a href=\"#%s\">Click Here if %s</a>\n",
					ExtractText(currentLine_f, "target=\"*", "\""), ExtractText(currentLine_f, "cond=\"f.", "\""));
		}
		else 
		{
			fprintf(NewFile, "<a href=\"#%s\">Click Here</a>\n",
					ExtractText(currentLine_f, "target=\"*", "\"") );
		}
	}
	//goes here if the line starts with |*select|
	else if( (currentLine_f[0] == '*') &&  (currentLine_f[1] == 's') && (currentLine_f[2] == 'e') && (currentLine_f[3] == 'l') )
	{
		//creates a jump point with id
		fprintf(NewFile, "<p id=\"%s\">------------------------------------------------------------</p>\n", ExtractText(currentLine_f, "*", "\n") );
	}
	//goes here if the line starts with |exlink txt| and creates a jump link
	else if(strstr(currentLine_f, "exlink txt="))
	{
		//goes here if there is a flag in the line
		if(strstr(currentLine_f, "exp="))
		{
			fprintf(NewFile, "<a href=\"#%s\">%s  [%s]</a><br><br>\n",
				ExtractText(currentLine_f, "target=\"*", "\""), ExtractText(currentLine_f, "exlink txt=", " target="), ExtractText(currentLine_f, "exp=\"f.", "\"" ) );
		}
		else
		{
			fprintf(NewFile, "<a href=\"#%s\">%s</a><br><br>\n",
				ExtractText(currentLine_f, "target=\"*", "\""), ExtractText(currentLine_f, "exlink txt=", " target=") );
		}
	}
	//free(currentLine_f);
}

void PrintCharacterName(char *currentLine_f)
{
	char ch_name[30] = "";
	bool has_name = false;

	if(strstr(currentLine_f, "t=\"京介\""))
	{
		strcat(ch_name, "Azai Kyousuke");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=tub"))
	{
		strcat(ch_name, "Tsubaki");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=eii"))
	{
		strcat(ch_name, "Eiichi");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=kan"))
	{
		strcat(ch_name, "Kanon");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=onn"))
	{
		strcat(ch_name, "Person");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=har"))
	{
		strcat(ch_name, "Haru");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=kot"))
	{
		strcat(ch_name, "Kotani");
		has_name = true;
	}
	/*else if(strstr(currentLine_f, ""))
	{
		strcat(ch_name, "Person");
		has_name = true;
	}*/

	if(has_name)
		fprintf(NewFile, "\n<p class=\"name\"><b><i>%s-</i></b></p>\n", ch_name);

	strcpy(ch_name, "");
}

void LineCleanup(char *currentLine_f)
{
	int i, len;
	char subline[3000];

	i = 0;
	len = strlen(currentLine_f);

	for( ;i < len; i++)
	{
		if(currentLine_f[i] == '[')
		{
			while(true)
			{
				i++;
				if(currentLine_f[i] == ']')
				{
					i++;
					break;
				}
			}
		}
		
		if(currentLine_f[i] != '\n')
			strncat(subline, &currentLine_f[i], 1);
	}

	fprintf(NewFile, "<p>%s</p>\n", subline);
	strcpy(subline, "");
}
