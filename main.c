/* main.c */

//int main (void)
//{
//	        print_hello ();
//}

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>

#define WND_X 0
#define WND_Y 0
#define WND_WDT 100
#define WND_HGH 100
#define WND_MIN_WDT 50
#define WND_MIN_HGH 50
#define WND_BORDER_WDT 5
#define WND_TITLE "Hello!"
#define WND_ICON_TITLE "Hello!"
#define PRG_CLASS "Hello!"

/*
* SetWindowManagerHints - процедура передает
информацию о
* свойствах программы менеджеру окон.
*/

static void SetWindowManagerHints (Display * prDisplay, /*Указатель на структуру Display */
								   char * psPrgClass, /*Класс программы */
								   char * argv[], /*Аргументы программы */
								   int argc, /*Число аргументов */
								   Window nWnd, /*Идентификатор окна */
								   int x, /*Координаты левого верхнего*/
								   int y, /*угла окна */
								   int nWidth,
								   int nHeight, /*Ширина и высота окна */
								   int nMinWidth,
								   int nMinHeight, /*Минимальные ширина и высота окна */
								   char * psTitle, /*Заголовок окна */
								   char * psIconTitle, /*Заголовок пиктограммы окна */
								   Pixmap nIconPixmap /*Рисунок пиктограммы */
								   )
{
	XSizeHints rSizeHints; /*Рекомендации о размерах окна*/
	XWMHints rWMHints;
	XClassHint rClassHint;
	XTextProperty prWindowName, prIconName;
	if ( !XStringListToTextProperty (&psTitle, 1,&prWindowName ) || !XStringListToTextProperty (&psIconTitle,1, &prIconName ) )
	{
		puts ( "No memory!\n");
		exit ( 1 );
	}
	rSizeHints.flags = PPosition | PSize |
	PMinSize;
	rSizeHints.min_width = nMinWidth;
	rSizeHints.min_height = nMinHeight;
	rWMHints.flags = StateHint | IconPixmapHint |
	InputHint;
	rWMHints.initial_state = NormalState;
	rWMHints.input = True;
	rWMHints.icon_pixmap= nIconPixmap;
	rClassHint.res_name = argv[0];
	rClassHint.res_class = psPrgClass;
	XSetWMProperties ( prDisplay, nWnd, &prWindowName, &prIconName, argv, argc, &rSizeHints, &rWMHints,
	&rClassHint );
}

/*
*main - основная процедура программы
*/

void main(int argc, char *argv[])
{
	Display *prDisplay; /* Указатель на структуру Display */
	int nScreenNum; /* Номер экрана */
	GC prGC;
	XEvent rEvent;
	Window nWnd;
	/* Устанавливаем связь с сервером */
	if ( ( prDisplay = XOpenDisplay ( NULL ) ) == NULL )
	{
		puts ("Can not connect to the X server!\n");
		exit ( 1 );
	}
	/* Получаем номер основного экрана */
	nScreenNum = DefaultScreen ( prDisplay );
	/* Создаем окно */
	nWnd = XCreateSimpleWindow ( prDisplay,
								RootWindow ( prDisplay, nScreenNum ),
								WND_X,
								WND_Y,
								WND_WDT,
								WND_HGH,
								WND_BORDER_WDT,
								BlackPixel ( prDisplay, nScreenNum ),
								WhitePixel ( prDisplay, nScreenNum ) );
	/* Задаем рекомендации для менеджера окон */
	SetWindowManagerHints ( prDisplay,
							PRG_CLASS,
							argv,
							argc,
							nWnd,
							WND_X,
							WND_Y,
							WND_WDT,
							WND_HGH,
							WND_MIN_WDT,
							WND_MIN_HGH,
							WND_TITLE,
							WND_ICON_TITLE,
							0
						   );
	/* Выбираем события, обрабатываемые программой */
	XSelectInput ( prDisplay, nWnd, ExposureMask | KeyPressMask );
	/* Показываем окно */
	XMapWindow ( prDisplay, nWnd );
	/* Цикл получения и обработки событий */
	while ( 1 ) {
		XNextEvent ( prDisplay, &rEvent );
		switch ( rEvent.type ) {
			case Expose :
				/* Запрос на перерисовку */
				if ( rEvent.xexpose.count != 0 ) break;
				prGC = XCreateGC ( prDisplay, nWnd, 0 , NULL );
				XSetForeground ( prDisplay, prGC, BlackPixel ( prDisplay, 0) );
				XDrawString ( prDisplay, nWnd, prGC, 10, 50, "Hello, world!", strlen ( "Hello, world!" ) );
				XFreeGC ( prDisplay, prGC );
				break;
			case KeyPress :
				/* Нажатие клавиши клавиатуры */
				XCloseDisplay ( prDisplay );
				exit ( 0 );
		}
	}
}