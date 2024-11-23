#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "dry_ctrl.hpp"


/* for git
https://javarush.com/groups/posts/2693-komandnaja-rabota-bez-putanicih-razbiraem-strategii-vetvlenija-v-gite?ysclid=m3pyocv94g291182151
https://javarush.com/groups/posts/2683-nachalo-rabotih-s-git-podrobnihy-gayd-dlja-novichkov
*/

int main(int argc, char* argv[]) {
    DryMdl mdl = DryMdl();
    //DryCtrl ctrl = DryCtrl();
    float dt = 0.001;
    char sstr[32] = "asd.log";
    //float uM=0;
    
    FILE * fw = fopen(sstr, "w+");
    int ticks = int(10.0 / dt);
    for (int i = 0; i < ticks; ++i)
    {
        //if(i>1000) setVi = -2000; if(i>2000) setVi = 0;
        //uM = ctrl.updateCtrl(dt, setVi, mdl.getFi(), mdl.getVi() );
        //uM = ctrl.updateCtrl(dt, setVi, ctrl.getFi(), ctrl.getVi() );
        mdl.updateMdl(dt, 0, 1);
        //fprintf( fw, "%f %f %s %f %s\n", dt*i, uM*0.1, mdl.print(), ctrl.GetUi()*0.1, ctrl.print() );
        fprintf( fw, "%f %s\n", dt*i, mdl.print());
    }
    fclose(fw);
// рисуем
   FILE *gp = popen("gnuplot -persist","w"); // gp - дескриптор канала
    if (gp == NULL)
    {
        printf("Error opening pipe to GNU plot.\n");
        return 0;
    }
    if(1){
      fprintf(gp, "\
  datafile = \"asd.log\"\n\
  set terminal png font arial 20 size 1024,600\n\
  set output \"aT1.png\"\n\
  set grid x y\n\
  plot \
  datafile using 1:4 title \"mVi\" w li lw 2 lc rgb \"#000080\",\
  datafile using 1:2 title \"uM\" w li lw 2 lc rgb \"#800000\",\
  datafile using 1:3 title \"mFi\" w li lw 2 lc rgb \"#008080\"\n\
");
    }
    if(0){
      fprintf(gp, "\
  datafile = \"asd.log\"\n\
  set terminal png font arial 20 size 1024,600\n\
  set output \"aT1.png\"\n\
  set grid x y\n\
  plot \
  datafile using 1:3 title \"mFi\" w li lw 2 lc rgb \"#408080\",\
  datafile using 1:4 title \"mVi\" w li lw 2 lc rgb \"#804080\",\
  datafile using 1:7 title \"kFi\" w li lw 2 lc rgb \"#400000\",\
  datafile using 1:8 title \"kVi\" w li lw 2 lc rgb \"#400000\"\n\
");
    }
    pclose(gp);
  return 0;
}

