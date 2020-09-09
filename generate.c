/* THIS IS THE generate.c FILE */
 init_output_page()
{
fprintf(fpout, "\n\n\n\n\n");
fflush(fpout);
}

//For Section
void  generate_sec_header(i, s)
int  i;
char *s;
{
fprintf(fpout, "\n\n%d %s\n", i, s);
fflush(fpout);

if (get_gen_toc() == TOC_ON)
   fprintf(fptoc, "\n%d %s ---------- PAGE %d\n", 
                            i, s, get_page_no());
}

//For Subsection
void  generate_subsec_header(i, j, s)
int  i,j;
char *s;
{
fprintf(fpout, "\n\n%d.%d %s\n", i, j, s);
fflush(fpout);

if (get_gen_toc() == TOC_ON)
   fprintf(fptoc, "\n%d.%d %s ---------- PAGE %d\n", 
                            i, j, s, get_page_no());
}

//For formatted texts
void  generate_formatted_text(s)
char *s;
{
int slen = strlen(s);
int i, j, k, r;
int llen;

for (i = 0; i <= slen; )
  {
   for (j = 0; ((j < OUT_WIDTH) && (i <= slen)); i++, j++)
       line[j] = s[i];

   if (i <= slen)
     {
       if ((line[j-1] != ' ') && (s[i] !=' '))
         {
           for (k = j-1; line[k] != ' '; k--)
             ;
           i = i - (j - k - 1);
           j = k;
         }
       for ( ; s[i] == ' '; i++) 
         ;
     }

   line[j] = '\0';

   llen = strlen(line);
   
   if (i <= slen)
     {
       fprintf(fpout, "\n%s", line);
       fflush(fpout);
     }
   else
     {
       for(r = 0; r <= llen; r++)
         s[r] = line[r];  /* includes backslash 0 */
     }
  }
}

// Prints blank lines
void print_blank_ln() {
  fprintf(fpout, "\n");
}

// Prints n vertical spaces to fpout
// For vertical spacing
void gen_vertical_space(int n) {
    for (int i = 0; i<n; i++)  {
      if (check_done_page()) {
        char page_no[5];
        strcpy(page_no, get_page_no());
        print_page_no(page_no);
      }
        fprintf(fpout,"\n");
        incr_lines_so_far;
    }
}

// Prints n horizontal spaces to fpout
void gen_horizontal_space(int n) {
    for (int i = 0; i<n; i++)  {
        fprintf(fpout," ");
    }
}

//For itemize
void  gen_item_text(s)
        char s[];
{
  char item_text[strlen(s)+8]; //Increases size of char[s] by 8 to add spacing
  for (int i =0; i<6; i++) {
    item_text[i] = ' '; //First 6 characters will be spaces
  }
  int i = 6;
  item_text[i++] = '-'; //7th char will be a -
  item_text[i++] = ' '; //8th char will be a space
  item_text[i++] = ' '; //9th char will be a space
  int j = 0;
  while (s[j]) {
    item_text[i++] = s[j]; //Rest of buffer will be filled with the argument passed through
    j++;
  }
  item_text[i++] = '\0';
  int slen = strlen(item_text); //Initiate buffer with same length as item_text
  int k, r; //Initialize integers k and r
  int llen; //Initialize integer llen

  for (i = 0; i <= slen; ) //Loop for length of item_text
  {
    for (j = 0; ((j < OUT_WIDTH) && (i <= slen)); i++, j++) //Prints out until j reaches OUT_WIDTH defined in util.c
      line[j] = item_text[i];
    if (i <= slen)
    {
      if ((line[j-1] != ' ') && (item_text[i] !=' '))
      {
        for (k = j-1; line[k] != ' '; k--)
          ;
        i = i - (j - k - 1);
        j = k;
      }
    }

    line[j] = '\0';
    llen = strlen(line);

    if (i <= slen+1)
    {
      if (!is_empty(line)) {
        if (check_done_page()) {
          char page_no[5];
          strcpy(page_no, get_page_no());
          print_page_no(page_no);
        }
        fprintf(fpout, "%s%s", line, get_line_spacing());
        incr_lines_so_far();
        for (int i = 0; i<get_ls(); i++) {
          incr_lines_so_far();
        }
      }
      fflush(fpout);
    }
    else
    {
      for(r = 0; r <= llen; r++)
        s[r] = line[r];  /* includes backslash 0 */
    }
  }
}

//Check if input str is empty for enumerate and itemize
int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

//Prints page number
//Makes a buffer of 40, first 20 filled with spaces, second 20 is argument passed through, char no[].
//Changing the size of page_no[] will affect how many spaces are before the page number.
void print_page_no(char no[]) {
  char page_no[40];
  for (int i=0; i<20; i++) {
    page_no[i] = ' ';
  }
  int i = 20;
  int j = 0;
  while(no[j]) {
    page_no[i] = no[j];
    i++;
    j++;
  }
  page_no[i] = '\0';
  fprintf(fpout, page_no);
  fprintf(fpout, "\n");
  init_lines_so_far();
  inc_page_no();

}

//Prints itemized text to fpout
void  gen_item_text(s)
        char s[];
{
  char item_text[strlen(s)+8]; //Increases size of char[s] by 8 to add spacing
  for (int i =0; i<6; i++) {
    item_text[i] = ' '; //First 6 characters will be spaces
  }
  int i = 6;
  item_text[i++] = '-'; //7th char will be a -
  item_text[i++] = ' '; //8th char will be a space
  item_text[i++] = ' '; //9th char will be a space
  int j = 0;
  while (s[j]) {
    item_text[i++] = s[j];
    j++;
  }
  item_text[i++] = '\0';
  int slen = strlen(item_text);
  int k, r;
  int llen;

  for (i = 0; i <= slen; ) //Loop for length of item_text
  {
    for (j = 0; ((j < OUT_WIDTH) && (i <= slen)); i++, j++) //Prints out until j reaches OUT_WIDTH defined in util.c
      line[j] = item_text[i];
    if (i <= slen)
    {
      if ((line[j-1] != ' ') && (item_text[i] !=' '))
      {
        for (k = j-1; line[k] != ' '; k--)
          ;
        i = i - (j - k - 1);
        j = k;
      }
    }

    line[j] = '\0';
    llen = strlen(line);

    if (i <= slen+1)
    {
      if (!is_empty(line)) {
        if (check_done_page()) {
          char page_no[5];
          strcpy(page_no, get_page_no());
          print_page_no(page_no);
        }
        fprintf(fpout, "%s%s", line, get_line_spacing());
        incr_lines_so_far();
        for (int i = 0; i<get_ls(); i++) {
          incr_lines_so_far();
        }
      }
      fflush(fpout);
    }
    else
    {
      for(r = 0; r <= llen; r++)
        s[r] = line[r];  /* includes backslash 0 */
    }
  }
}