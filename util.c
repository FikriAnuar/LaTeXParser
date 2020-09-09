/* THIS IS THE util.c FILE */

FILE *fpout;
FILE *fptoc;

#define  OUT_WIDTH       40
#define  SPACE_LEFT       5
#define  LINES_PER_PAGE  40
#define  TOC_ON           1

char  line[OUT_WIDTH + 1];
int   lines_so_far;

void  init_lines_so_far()
{
  lines_so_far = 0;
}

void  incr_lines_so_far()
{
  lines_so_far++;
}

int   check_done_page()
{
  if (lines_so_far >= LINES_PER_PAGE) 
    return 1;
  else
    return 0;
}


struct  doc_symtab 
   {
      int     page_no_counter;
      int     page_style;
      int     line_spacing;
      int     current_font;
      int     generate_toc;
      int     section_counter;
      int     subsect_counter;
	  int     ls; //Line Spacing integer
	  int	  enum_flag; //Flag for enumerate
	  int     itemize_flag; //Flag for itemize
   };

struct  doc_symtab  DST;

void  init_sec_ctr()
{
  DST.section_counter = 1;
  DST.subsect_counter = 1;
  DST.line_spacing = "\n";
}

void  incr_sec_ctr()
{
  DST.section_counter++;
  DST.subsect_counter = 1;
}

void  incr_subsec_ctr()
{
  DST.subsect_counter++;
}

int  get_sec_ctr()
{
  return DST.section_counter;
}

int  get_subsec_ctr()
{
  return DST.subsect_counter;
}

int   get_gen_toc()
{
  return  DST.generate_toc;
}

void  set_gen_toc()
{
  DST.generate_toc = 1;
}

void  set_page_no(p)
char  p;
{
  DST.page_no_counter = p - '0';
}

int   get_page_no(p)
{
  return DST.page_no_counter;
}


int   inc_page_no()
{
  DST.page_no_counter++;
  return (DST.page_no_counter - 1);
}

//Sets page style
void  set_page_style(s)
int   s;
{
  DST.page_style = s;
}

// Get the int value of line spacing
int get_ls() {
  return DST.ls;
}

// Set the line spacing
void set_line_spacing(char s) {
    if (s=='1') {
        DST.line_spacing = "\n"; //Single Spacing
        DST.ls = 1;
    } else if (s=='2') {
        DST.line_spacing = "\n\n"; //Double Spacing
        DST.ls = 2;
    } else if (s=='3') {
        DST.line_spacing = "\n\n\n"; //Triple Spacing
        DST.ls = 3;
    } else {
        DST.line_spacing = "\n"; //Default Spacing
        DST.ls = 1;
    }
}

//Set the itemize flag
void set_itemize_flag(int i) {
  DST.itemize_flag = i;
}

// Get the itemize flag
int get_itemize_flag() {
  return DST.itemize_flag;
}

// Get the enumerate flag value
int get_enum_flag() {
  return DST.single_flag;
}

// Set the enumerate flag value
void set_enum_flag(int flag) {
  DST.single_flag = flag;
}