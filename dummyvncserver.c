
#include <unistd.h>
#include <rfb/rfb.h>
#include <rfb/keysym.h>

static const int bpp=4;
static int maxx=800, maxy=600;
static int vnc_to_exit = 0;

static void initBuffer(unsigned char* buffer, unsigned char x)
{
  int i,j;
  for(j=0;j<maxy;++j) {
    for(i=0;i<maxx;++i) {
      buffer[(j*maxx+i)*bpp+0]=(i+j)*128/(maxx+maxy) + x; /* red */
      buffer[(j*maxx+i)*bpp+1]=i*128/maxx + x; /* green */
      buffer[(j*maxx+i)*bpp+2]=j*256/maxy + x; /* blue */
    }
    buffer[j*maxx*bpp+0]=0xff;
    buffer[j*maxx*bpp+1]=0xff;
    buffer[j*maxx*bpp+2]=0xff;
    buffer[j*maxx*bpp+3]=0xff;
  }
}

/* Here the key events are handled */
static void dokey(rfbBool down,rfbKeySym key,rfbClientPtr cl)
{
  if(down) {
    if(key==XK_Escape)
      /* close down server, disconnecting clients */
      rfbShutdownServer(cl->screen,TRUE);
      vnc_to_exit = 1;
  }
}

void render(rfbScreenInfoPtr rfbScreen, unsigned char x) {
   initBuffer((unsigned char*)rfbScreen->frameBuffer, x);
   rfbMarkRectAsModified(rfbScreen,0,0,800,600);
}

/* Initialization */
int main(int argc,char** argv)
{
  rfbScreenInfoPtr rfbScreen = rfbGetScreen(NULL,NULL,maxx,maxy,8,3,bpp);
  if(!rfbScreen)
    return 0;
  rfbScreen->desktopName = "MIR Screen";
  rfbScreen->frameBuffer = (char*)malloc(maxx*maxy*bpp);
  rfbScreen->alwaysShared = TRUE;
  rfbScreen->kbdAddEvent = dokey;
  initBuffer((unsigned char*)rfbScreen->frameBuffer, 0);
  /* initialize the server */
  rfbInitServer(rfbScreen);

  /* this is the non-blocking event loop; a background thread is started */
  rfbRunEventLoop(rfbScreen,-1,TRUE);
  fprintf(stderr, "Running background loop...\n");
  unsigned char i = 0;
  while(! vnc_to_exit) {
	printf("render() %i\n", i);
	render(rfbScreen, i);
	i += 32;
	sleep(1);
  }

  free(rfbScreen->frameBuffer);
  rfbScreenCleanup(rfbScreen);
  return(0);
}
