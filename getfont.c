/* getfont.c -- extract embedded PostScript font */
/* Peter Flass -- NYS LBDC -- July, 1994         */

#include <stdio.h>
#include <string.h>

main(int argc, char *argv[], char *envp[])
{
   char outfile[64];
   char infile[64];
   char buf[512], *b, *e;
   int arg;
   FILE *in,*out;
   int InFont=0;
   int AllFonts=0;
   int eq;
   const char BeginFont[] = "%%BeginFont: ";
   const char EndFont[]   = "%%EndFont";

   if(argc<2) {
      fprintf(stderr,"Usage: getfont infile [font ...]\n");
      exit(1);
   }
   strcpy(infile,argv[1]);
   in=fopen(infile,"r");
   if(in==NULL) {
      fprintf(stderr,"Unable to open input file %s\n",infile);
      exit(1);
   }
   if(argc<3) {
      AllFonts=1;
   }
   for(;;) {
      b=fgets(buf,512,in);
      if(feof(in)) break;
      if(b==NULL) {
         fprintf(stderr,"Error reading input file %s\n",infile);
         exit(1);
      }
      eq=strncmp(buf,EndFont,strlen(EndFont));
      if(eq==0) {
         if(InFont==1) {
            fputs(buf,out);
            InFont=0;
            fclose(out);
            fprintf(stderr,"Done\n");
         }
         continue;
      }

      if(InFont==1) {
         fputs(buf,out);
         continue;
      }

      eq=strncmp(buf,BeginFont,strlen(BeginFont));
      if(eq!=0) continue;
      b=&buf[0]+strlen(BeginFont);
      e=strpbrk(b," \r\n");
      if(e==NULL) e=b+strlen(b)+1;
      strncpy(outfile,b,e-b);
      *(outfile+(int)(e-b))='\0';
      for(arg=2;arg<argc;arg++) {
         if( (strcmp(argv[arg],outfile)==0) ) {
            InFont=1;
            break;
         }
      }
      if(AllFonts==1) InFont=1;
      if(InFont==1) {
         fprintf(stderr,"%s ...",outfile);
         strcat(outfile,".pfa");
         out=fopen(outfile,"w");
         if(out==NULL) {
            fprintf(stderr,"Unable to open output file %s\n",
                    outfile);
            exit(1);
         } /* endif */
         fputs(buf,out);
      } /* endif */
  } /* endfor */

fprintf(stderr,"Fini!\n");
exit(0);

}
