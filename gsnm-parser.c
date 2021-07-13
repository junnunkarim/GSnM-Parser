//Tsubaki Good End -g23 | flag_tubaki >= 2 | select1_1 | select2_1 |gt01.ks
//Tsubaki Bad End - g23 | flag_tubaki = 0 | end before 'select0'
//Kanon End - g34 | flag_kanon >= 1 |select1_2 | select2_end | gk01.ks
//Kanon Good End - gk07 | select2_1
//Kanon Bad End - gk07 | select2_2
//Mizuha End - g42 | flag_mizuha >= 1 | storage="gm01.ks"
//Mizuha Good End - gm04 | select1_2
//Mizuha Bad End - gm04 | select1_1


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

FILE *CurrentFile; //opens input files
FILE *NewFile; //opnes output files

bool ReadFile(char *file_name);
char *ExtractText(const char *const string, const char *const start, const char *const end);
void PrintTitle(char *currentLine_f); //prints chapter titles
void FileJumpTarget(char *file_name_f); //creates title jump links in different files
void FileJumpLink(char *currentLine_f); //creates ending jump links in different files
void JumpLink_Target(char *currentLine_f); //creates jump links and targets in same file
void PrintCharacterName(char *currentLine_f); // prints name of the character who is currently speaking
void LineCleanup(char *currentLine_f); //formats the line //cleans up unwanted characters or strings

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
  
 	if( (NewFile = fopen(new_file_name, "w")) == NULL )
	{
		exit(1);
	}
	
	//prints a jump target at the beginning of a file
	FileJumpTarget(file_name);
	
	//prints a chapter tag if the file is g01.ks because there aren't any identifier for chapter-1
	if(strstr(file_name, "g01.ks"))
		fprintf(NewFile, "<h2 class = \"chapter\">Chapter 1</h2>\n");

	//gets a newline in each iteration and validates that line until end of file is found
  while( (read = getline(&currentLine, &length, CurrentFile)) != EOF )
  {
		//printf("%s\n", currentLine); //for debugging
		
		//goes here if line contains |storage=cp| and prints a chapter tag
		if(strstr(currentLine, "storage=cp"))
			PrintTitle(currentLine);
		
		//creates jump links in different files
		if(strstr(currentLine, "@jump storage="))
			FileJumpLink(currentLine);
			
		//if the line contains choices than creates jump points
		if(strstr(currentLine, "*select"))
			JumpLink_Target(currentLine);
		
		//prints the name of the character who is currently speaking
		if(strstr(currentLine, "nm t="))
			PrintCharacterName(currentLine);
		
		//prints the line and after that loops to get a new line
		if(strstr(currentLine, "[np]") || strstr(currentLine, "[wvl]"))
			LineCleanup(currentLine);
  }
  
  fclose(NewFile);
  fclose(CurrentFile);

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

		//printf("%s\n", result); //for debugging

    return result;
}

void PrintTitle(char *currentLine_f)
{
	if(strstr(currentLine_f, "storage=cp_title02"))
		fprintf(NewFile, "<h2 class = \"chapter\">Chapter 2</h2>\n");
	else if(strstr(currentLine_f, "storage=cp_title03"))
		fprintf(NewFile, "<h2 class = \"chapter\">Chapter 3</h2>\n");
	else if(strstr(currentLine_f, "storage=cp_title04"))
		fprintf(NewFile, "<h2 class = \"chapter\">Chapter 4</h2>\n");
	else if(strstr(currentLine_f, "storage=cp_title05"))
		fprintf(NewFile, "<h2 class = \"chapter\">Chapter 5</h2>\n");
	else if(strstr(currentLine_f, "storage=cp_title06"))
		fprintf(NewFile, "<h2 class = \"chapter\">Chapter 6</h2>\n");
	else if(strstr(currentLine_f, "storage=cp_tubaki"))
		fprintf(NewFile, "<h2 class = \"chapter\">Tsubaki Good End</h2>\n");
	else if(strstr(currentLine_f, "storage=cp_kanon"))
		fprintf(NewFile, "<h2 class = \"chapter\">Kanon Good End</h2>\n");
	else if(strstr(currentLine_f, "storage=cp_mizuha"))
		fprintf(NewFile, "<h2 class = \"chapter\">Mizuha Good End</h2>\n");
}

void FileJumpTarget(char *file_name_f)
{
	if(strstr(file_name_f, "g01.ks"))
		fprintf(NewFile, "<p id=\"title\">--------------------</p>\n");
	else
		fprintf(NewFile, "<p id=\"%s\">--------------------</p>\n", ExtractText(file_name_f, "/", "."));
}

void FileJumpLink(char *currentLine_f)
{	
	if(strstr(currentLine_f, "@jump storage=\"title"))
	{
		fprintf(NewFile, "<i><b><a href=\"%s.html#%s\">End</a></b></i><br><br>\n", ExtractText(currentLine_f, "storage=\"", ".ks"), ExtractText(currentLine_f, "storage=\"", ".ks") );
	}
	else if(strstr(currentLine_f, "@jump storage=\"gk01"))
	{
		fprintf(NewFile, "<a href=\"%s.html#%s\">Kanon Ending</a><br><br>\n", ExtractText(currentLine_f, "storage=\"", ".ks"), ExtractText(currentLine_f, "storage=\"", ".ks") );
	}
	else if(strstr(currentLine_f, "@jump storage=\"gm01"))
	{
		fprintf(NewFile, "<a href=\"%s.html#%s\">Mizuha Ending</a><br><br>\n", ExtractText(currentLine_f, "storage=\"", ".ks"), ExtractText(currentLine_f, "storage=\"", ".ks") );
	}
	else if(strstr(currentLine_f, "@jump storage=\"gt01"))
	{
		fprintf(NewFile, "<a href=\"%s.html#%s\">Tsubaki Ending</a><br><br>\n", ExtractText(currentLine_f, "storage=\"", ".ks"), ExtractText(currentLine_f, "storage=\"", ".ks") );
	}
	else if(strstr(currentLine_f, "@jump storage="))
	{
		fprintf(NewFile, "<a href=\"%s.html#%s\">Next</a><br><br>\n", ExtractText(currentLine_f, "storage=\"", ".ks"), ExtractText(currentLine_f, "storage=\"", ".ks") );
	}
}

void JumpLink_Target(char *currentLine_f)
{
	//goes here if the line doesn't contain "exlink txt" and has "target" in it
	//an example would be this - |@jump target="*select0" cond="f.flag_tubaki>0"|
	if( strstr(currentLine_f, "target=\"") && !strstr(currentLine_f, "exlink txt=") )
	{
		//goes here if there is condition on the line
		if(strstr(currentLine_f, "cond="))
		{
			//creates a jump link with href
			fprintf(NewFile, "<a href=\"#%s\">Click Here if FLAG - %s</a>\n",
					ExtractText(currentLine_f, "target=\"*", "\""), ExtractText(currentLine_f, "cond=\"f.flag_", "\""));
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
		//creates a jump target with id
		fprintf(NewFile, "<p id=\"%s\">--------------------</p>\n", ExtractText(currentLine_f, "*", "\n") );
	}
	//goes here if the line starts with |exlink txt| and creates a jump link
	else if(strstr(currentLine_f, "exlink txt="))
	{
		//goes here if there is a flag in the line
		if(strstr(currentLine_f, "exp="))
		{
			fprintf(NewFile, "<a href=\"#%s\">%s  [FLAG - %s]</a><br><br>\n",
				ExtractText(currentLine_f, "target=\"*", "\""), ExtractText(currentLine_f, "exlink txt=", " target="), ExtractText(currentLine_f, "exp=\"f.flag_", "\"" ) );
		}
		else
		{
			fprintf(NewFile, "<a href=\"#%s\">%s</a><br><br>\n",
				ExtractText(currentLine_f, "target=\"*", "\""), ExtractText(currentLine_f, "exlink txt=", " target=") );
		}
	}
}

void PrintCharacterName(char *currentLine_f)
{
	char ch_name[30] = "";
	bool has_name = false;

	if(strstr(currentLine_f, "t=\"京介\""))
	{
		strcat(ch_name, "Azai");
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
		strcat(ch_name, "Miss Noriko");
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
	else if(strstr(currentLine_f, "=suu"))
	{
		strcat(ch_name, "Math Teacher");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=uwe"))
	{
		strcat(ch_name, "Cafe Waiter");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=gon"))
	{
		strcat(ch_name, "Gonzou");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=pro"))
	{
		strcat(ch_name, "Producer");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=miz"))
	{
		strcat(ch_name, "Mizuha");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=som"))
	{
		strcat(ch_name, "Someya");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=mao"))
	{
		strcat(ch_name, "Maou");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=kuw"))
	{
		strcat(ch_name, "Kuwashima");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=aki"))
	{
		strcat(ch_name, "Dr. Akimoto");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=pen"))
	{
		strcat(ch_name, "Penguin guy");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=ch1"))
	{
		strcat(ch_name, "Child 1");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=ch2"))
	{
		strcat(ch_name, "Child 2");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=ten"))
	{
		strcat(ch_name, "Manager");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=hir"))
	{
		strcat(ch_name, "Hiroaki");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=pap"))
	{
		strcat(ch_name, "Tsubaki's Father");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=mam"))
	{
		strcat(ch_name, "Tsubaki's Mother");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=tir"))
	{
		strcat(ch_name, "Chiromi");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=tka"))
	{
		strcat(ch_name, "Tsubaki's Sibling 1");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=sae"))
	{
		strcat(ch_name, "Tsubaki's Sibling 2");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=sai"))
	{
		strcat(ch_name, "Mephistopheles");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=yuu"))
	{
		strcat(ch_name, "Ikuko");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=yuk"))
	{
		strcat(ch_name, "Yuki");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=ooo"))
	{
		strcat(ch_name, "Kannu");
		has_name = true;
	}
	else if(strstr(currentLine_f, "=hah"))
	{
		strcat(ch_name, "Mom");
		has_name = true;
	}
	else
	{
		strcat(ch_name, "Person");
		has_name = true;
	}
	

	if(has_name)
		fprintf(NewFile, "\n<p class=\"name\"><b><i>%s-</i></b></p>\n", ch_name);

	strcpy(ch_name, "");
}

void LineCleanup(char *currentLine_f)
{
	int i, len;
	char subline[3000];

	len = strlen(currentLine_f);

	for(i = 0;i < len; i++)
	{
		//enters if |[| is found and skips characters until |]| is found
		if(currentLine_f[i] == '[')
		{
			//this loop only breaks after |]| is found
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
