/* THIS IS latexp3code.y */
%{ /* A YACC FOR PART 3 OF THE PROJECT WHERE VERBATIM AND NESTING WORKS */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define  BUF_SIZE       512

int ws_flag = 0;

char ls_value[10];
char init_spacing; %%For initial spacing
char itemize_text[1000]; //For itemize/enumerate
char enum_text[1000];
int ordering_flag; //To distinguish between itemize and enumerate

#include "lex.yy.c"
#include "util.c"
#include "generate.c"

%}


%union 
   {
      char   trans[BUF_SIZE+1];
      int    val;
   }

%start latexstatement

%token  BACKSL    LBEGIN    LCURLYB    DOCUMENT    RCURLYB    END
%token  WORD      WS        SPECCHAR   CENTER      VERBATIM   SINGLE  
%token  ITEMIZE   ENUMERATE TABULAR    TABLE       LSQRB      RSQRB
%token  H         T         B          COLS
%token  CAPTION   LABEL     DBLBS      ITEM        SECTION    SUBSEC  
%token  TABOCON   RENEW     BASELINES  PAGENUM     INTEGER    ARABIC1 
%token  LROMAN1   CROMAN1   LALPH1     CALPH1      VSPACE     HSPACE
%token  RM        IT        NOINDENT   REF 
%token  ARABIC2   LROMAN2   CROMAN2    LALPH2      CALPH2

%type <trans> textoption  wsorword WS WORD linespacing
%type <val> style2 ARABIC2 LROMAN2 CROMAN2 LALPH2 CALPH2

%%
latexstatement   :  startdoc  mainbody  enddoc
                 ;

startdoc         :  LBEGIN  DOCUMENT  
                 ;

enddoc           :  END  DOCUMENT  
                 ;

mainbody         :  mainbody  mainoption
                 |  mainoption
                 ;

mainoption       :  textoption
                    {
                      generate_formatted_text($1);
                    }
                 |  commentoption
                 |  latexoptions
                 ;

textoption       :  textoption  wsorword
                    {
                      strcat($$, " ");
                      strcat($$, $2);
                    }
                 |  wsorword
                    {
                      strcpy($$, $1);
                    }
                 ;

wsorword         :  WS 
                    {
                      strcpy($$, yytext);
                    }
                 |  WORD
                    {
                      strcpy($$, yytext);
                    }
                 ;

commentoption    :  SPECCHAR  textoption
                 ;

latexoptions     :  backsoptions
                 |  LCURLYB  curlyboptions  RCURLYB
                 ;

curlyboptions    :  fonts  textoption
                 ;

backsoptions     :  beginendopts
                 |  sectionoptions
                 |  tableofcont
                 |  linespacing
                 |  pagenumbers
                 |  pagenuminit
                 |  spacing
                 |  fonts
                 |  specialchar
                 |  nonewpara
                 |  reference
                 ;
     
beginendopts     :  LBEGIN  begcmds  beginblock  endbegin  
                 ;

begcmds          :  CENTER  
                 |  VERBATIM  {ws_flag=1;}
                 |  SINGLE	{
                          init_spacing = get_ls()+'0'; //For Line Spacing
                          set_line_spacing('1');
                          }  
                 |  ITEMIZE {set_itemize_flag(1);
							if (get_enum_flag()) {
							ordering_flag = 1;
							}
							}
                 |  ENUMERATE {set_itemize_flag(1);
								if (get_enum_flag()) {
								  ordering_flag = 2;
								}
							  }
                 |  TABLE  begtableopts
                 |  TABULAR  begtabularopts
                 ;

endbegin         :  END  endcmds
                 |  endtableopts  TABLE  
                 ;

endcmds          :  CENTER  
                 |  VERBATIM  {ws_flag=0;}
                 |  SINGLE  {set_line_spacing(init_spacing);} //Return spacing to normal
                 |  ITEMIZE  {set_itemize_flag(0); ordering_flag = 0;} //Return flag back to 0
                 |  ENUMERATE {set_enum_flag(0); reset_enum_numbering(); print_blank_ln(); ordering_flag = 0;} //Return flag back to 0 and prints blank line
                 |  TABULAR
                 ;

beginblock       :  beginendopts
                 |  textoption /* FOR single or verbatim */
                                    {printf("single or verb\n");}
                 |  entrylist  /* FOR center and tabular */
                                    {printf("center or tabular\n");}
                 |  listblock  /* FOR item and enumerate */
                                    {printf("item or enumerate\n");}
                 ;

listblock        :  listblock  anitem
                                    {printf("listblockA\n");}
                 |  anitem
                                    {printf("listblockB\n");}
                 ;

anitem           :  ITEM  textoption
				 {
				 if (ordering_flag) {
					  strcat(itemize_text, $2);
					  gen_item_text(itemize_text);
					  memset(itemize_text, 0, sizeof itemize_text); //Set first sizeof_itemize_text to 0 in itemize_text
				  } else if (ordering_flag == 2) {
					  strcat(enum_text, $2);
    				  gen_enum_text(enum_text);
    				  incr_enum_numbering();
    				  memset(enum_text, 0, sizeof enum_text); //Set first sizeof_enum_text to 0 in itemize_text
  				  } else if (get_itemize_flag()) {
    				  strcat(itemize_text, $2);
    				  gen_item_text(itemize_text);
				  memset(itemize_text, 0, sizeof itemize_text); //Set first sizeof_itemize_text to 0 in itemize_text
  				  } else if (get_enum_flag()) {
    				  strcat(enum_text, $2);
    				  gen_enum_text(enum_text);
    				  incr_enum_numbering();
    				  memset(enum_text, 0, sizeof enum_text);
  				  }
				 }
                 |  beginendopts
                 ;

entrylist        :  entrylist  anentry
                                    {printf("entrylistA\n");}
                 |  anentry
                                    {printf("entrylistB\n");}
                 ;

anentry          :  entry  DBLBS
                                    {printf("anentryA\n");}
                 |  beginendopts
                                    {printf("anentryB\n");}
                 ;

entry            :  entry  SPECCHAR  textoption
                                    {printf("entryA\n");}
                 |  textoption
                                    {printf("entryB\n");}
                 ;

begtableopts     :  LSQRB  position  RSQRB
                 ;
                 
begtabularopts   :  LCURLYB  COLS  RCURLYB
                 ;

position         :  H  
                 |  T  
                 |  B
                 ;

endtableopts     :  END
                 |  CAPTION  LCURLYB  textoption  RCURLYB  captionrest
                 |  labelrest 
                 ;

captionrest      :  END
                 |  labelrest
                 ;

labelrest        :  LABEL  LCURLYB  WORD  RCURLYB  END
                 ;

sectionoptions   :  SECTION  LCURLYB  textoption  RCURLYB
                    {
                      generate_sec_header(get_sec_ctr(), $3);
                      incr_sec_ctr();
                    }
                 |  SUBSEC  LCURLYB  textoption  RCURLYB
                    {
                      generate_subsec_header(get_sec_ctr(), get_subsec_ctr(), $3);
                      incr_subsec_ctr();
                    }
                 ;

tableofcont      :  TABOCON
                    {
                      set_gen_toc();
                    }
                 ;

linespacing      :  RENEW  LCURLYB  BASELINES  RCURLYB
                            LCURLYB  WORD  
                    {
                      strcpy(ls_value, yytext);
                      printf("WORD IS: %s\n",ls_value);
                    }
						RCURLYB
                    {
                      strcpy($$, ls_value);
                    }
                 ;

pagenumbers      :  PAGENUM  style2
                    {
                      set_page_style($2);
                    }
                 ;

style2           :  ARABIC2
                 |  LROMAN2 
                 |  CROMAN2 
                 |  LALPH2
                 |  CALPH2
                 ;

pagenuminit      :  style1  LCURLYB  WORD  
                    {
                      set_page_no(yytext[0]);
                    }
                    RCURLYB
                 ;

style1           :  ARABIC1
                 |  LROMAN1 
                 |  CROMAN1 
                 |  LALPH1 
                 |  CALPH1
                 ;

spacing          :  horvert  LCURLYB  WORD  RCURLYB
                 ;

horvert          :  VSPACE  
                 |  HSPACE
                 ;

fonts            :  RM  
                 |  IT
                 ;

specialchar      :  SPECCHAR  
                 |  LCURLYB  
                 |  RCURLYB
                 ;

nonewpara        :  NOINDENT
                 ;

reference        :  REF  LCURLYB  WORD  RCURLYB
                 ;
%%

yyerror(){}

main()
{
fpout = fopen("latexout","w");
fptoc = fopen("latextoc","w");
init_lines_so_far();
init_sec_ctr();
init_output_page();

yyparse();
}
