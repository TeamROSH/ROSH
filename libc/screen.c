#include "screen.h"
#include "string.h"
#include "memory.h"
#include "../kernel/ports.h"
#include "../kernel/IDT/keyboard.h"
#include "../kernel/memory/heap.h"
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define TAB 4
#define FALSE 0
#define TRUE !FALSE
#define UP TRUE
#define DOWN FALSE
#define NULL 0
#define SCROLL_COLS COLS * 2

char* getaddr();
void moveCursor(int n);
int get_cursor_position(void);
int print_special(char c);
int printable(char c);
void up_putc(char c);
char key_replacement(char c);
void scrollScreen(int direction);

int cursor = 0;
char* screenTrackerUp = NULL;
char* screenTrackerDown = NULL;
int linesUp = 0;
int linesDown = 0;

char key_flags[] = {FALSE, FALSE, FALSE, FALSE};		// ctrl shift alt CapsLock
char shift_replacements[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
							21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, ' ', '!', '"', '#', '$', '%',
							'&', '\'', '(', ')', '*', '+', '<', '_', '>', '?', ')', '!', '@', '#', '$',
							'%', '^', '&', '*', '(', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D',
							'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
							'U', 'V', 'W', 'X', 'Y', 'Z', '{', '|', '}', '^', '_', '~', 'A', 'B', 'C', 'D',
							'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
							'U', 'V', 'W', 'X', 'Y', 'Z', '{', '|', '}', '~', 0};

void putc(char c)
{
	if (!print_special(c) && cursor < ROWS * COLS)
		up_putc(key_replacement(c));
	if (cursor == ROWS * COLS)
	{
		moveCursor(-COLS);
		scrollScreen(DOWN);
	}
}

/*
	get pointer of screen cursor
	@returns pointer of char
*/
char* getaddr()
{
	return (char*)SCREEN + 2 * cursor;		// get pointer by cursor
}

/*
	move cursor n steps
	@param n: number of steps to move
*/
void moveCursor(int n)
{
	cursor += n;
	outb(REG_SCREEN_CTRL, 15);		// update cursor on screen
    outb(REG_SCREEN_DATA, (uint8_t)(cursor & 0xff));
	outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, (uint8_t)((cursor >> 8) & 0xff));
}

void puts(char* str)
{
	int len = strlen(str);		// get string length
	for (int i = 0; i < len; i++)		// for every char in string
	{
		putc(str[i]);		// print it
	}
}

void puti(int n)
{
	int size = digits(n), temp = 0;
	for (int i = 0; i < size; i++)		// for every digit
	{
		temp = n;
		for (int j = 0; j < size - i - 1; j++)		// get digit
			temp /= 10;
		putc((char)(temp % 10 + ASCII_NUM_OFFSET));		// print it
	}
}

int get_cursor_position(void)
{
    int pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((int)inb(0x3D5)) << 8;
    return pos;
}

void initConsole()
{
	cursor = get_cursor_position();
}

/*
	print a special character
	@param c: the char to print
	@returns was special character printed
*/
int print_special(char c)
{
	switch (c)
	{
		case '\n':
			if (cursor / COLS + 1 < ROWS)
				moveCursor(COLS - (cursor % COLS));		// set cursor to start of next line
			else
			{
				moveCursor(-(cursor % COLS));
				scrollScreen(DOWN);
			}
			return TRUE;
		case '\t':
			if (cursor < ROWS * COLS - TAB)		// if room of tab
				moveCursor(TAB);
			else
			{
				moveCursor(TAB -COLS);
				scrollScreen(DOWN);
			}
			return TRUE;
		case '\r':
			moveCursor(-(cursor % COLS));		// move to the start of the line
			return TRUE;
		case '\b':
			if (cursor > 0)
			{
				moveCursor(-1);		// delete last insert
				up_putc('\0');
				moveCursor(-1);
			}
			else if (linesUp > 0)
			{
				moveCursor(COLS - 1);
				scrollScreen(UP);
			}
			return TRUE;
		default:
			return !printable(c);
	}
}

/*
	return if char is printable
	@param c: the char to print
	@returns is printable
*/
int printable(char c)
{
	return c >= 32 && c <= 126;		// between space and ~ in ascii table
}

/*
	print a char to the screen (private unprotected version)
	@param c: char to print
*/
void up_putc(char c)
{
	char* pos = getaddr();		// get pos of char
	pos[0] = c;		// print char
	moveCursor(1);
}

void non_char_print(uint8_t c)
{
	switch (c)
	{
		case PAGEUP_SYMBOL:
			moveCursor(-cursor);		// bring to the top
			break;
		case PAGEDOWN_SYMBOL:
			moveCursor((ROWS - 1) * COLS - cursor);		// bring to last line
			break;
		case HOME_SYMBOL:
			moveCursor(-(cursor % COLS));		// move to the start of the line
			break;
		case END_SYMBOL:
			moveCursor(COLS - (cursor % COLS) - 1);		// set cursor to end of line
			break;
		case INSERT_SYMBOL:
			break;
		case DELETE_SYMBOL:
			if (cursor < ROWS * COLS - 1){
				up_putc('\0');
				moveCursor(-1);
			}
			break;
		case UP_SYMBOL:
			if (cursor >= COLS)
				moveCursor(-COLS);
			else if (linesUp > 0)
				scrollScreen(UP);
			break;
		case DOWN_SYMBOL:
			if (cursor <= ((ROWS - 1) * COLS - 1))
				moveCursor(COLS);
			else if (linesDown > 0)
				scrollScreen(DOWN);
			break;
		case LEFT_SYMBOL:
			if (cursor > 0)
				moveCursor(-1);
			else if (linesUp > 0)
				scrollScreen(UP);
			break;
		case RIGHT_SYMBOL:
			if (cursor < ROWS * COLS - 1)
				moveCursor(1);
			else if (linesDown > 0)
				scrollScreen(DOWN);
			break;
		case CTRL_PRESS:
			key_flags[0] = TRUE;
			break;
		case SHIFT_PRESS:
			key_flags[1] = TRUE;
			break;
		case ALT_PRESS:
			key_flags[2] = TRUE;
			break;
		case CAPSLOCK_PRESS:
			key_flags[3] = !key_flags[3];
			break;
		case CTRL_RELEASE:
			key_flags[0] = FALSE;
			break;
		case SHIFT_RELEASE:
			key_flags[1] = FALSE;
			break;
		case ALT_RELEASE:
			key_flags[2] = FALSE;
			break;
	}
}

/*
	get the correct char to print (Shift + key...)
	@param c: char got
	@returns actual char
*/
char key_replacement(char c)
{
	if (key_flags[1] && key_flags[3])		// shift & CapsLock
		if (c >= 97 && c <= 121)		// if lower case letter
			return c;
		else
			return shift_replacements[(int)c];
	else if (key_flags[1])		// shift
		return shift_replacements[(int)c];
	else if (key_flags[3])		// CapsLock
		if (c >= 97 && c <= 121)		// if lower case letter
			return shift_replacements[(int)c];
		else
			return c;
	else
		return c;

}

void clearConsole()
{
	char* screen = (char*)SCREEN;
	for (int i = 0; i < ROWS * COLS * 2; i += 2)
		screen[i] = 0;
	moveCursor(-cursor);
}

/*
	scroll screen up or down
	@param direction: Up (TRUE) or Down (FALSE)
*/
void scrollScreen(int direction)
{
	char* screen = (char*)SCREEN;
	if (direction == UP)
	{
		linesDown++;		// move rows down
		screenTrackerDown = (char*)krealloc(screenTrackerDown, linesDown * SCROLL_COLS);
		memcpy(screenTrackerDown + (linesDown - 1) * SCROLL_COLS, screen + (ROWS - 1) * SCROLL_COLS, SCROLL_COLS);
		for (int i = ROWS * SCROLL_COLS; i >= SCROLL_COLS; i--)		// opposite order copy
			screen[i] = screen[i - SCROLL_COLS];

		if (linesUp > 0)
		{
			linesUp--;		// get saved rows
			memcpy(screen, screenTrackerUp + linesUp * SCROLL_COLS, SCROLL_COLS);
			if (linesUp > 0)
				screenTrackerUp = (char*)krealloc(screenTrackerUp, linesUp * SCROLL_COLS);
			else
				kfree(screenTrackerUp);
		}
		else
		{
			for (int i = 0; i < SCROLL_COLS; i += 2)		// empty first row
				screen[i] = 0;
		}
	}
	else
	{
		linesUp++;		// move rows up
		screenTrackerUp = (char*)krealloc(screenTrackerUp, linesUp * SCROLL_COLS);
		memcpy(screenTrackerUp + (linesUp - 1) * SCROLL_COLS, screen, SCROLL_COLS);
		memcpy(screen, screen + SCROLL_COLS, (ROWS - 1) * SCROLL_COLS);		// order ok

		if (linesDown > 0)
		{
			linesDown--;		// get saved rows
			memcpy(screen + (ROWS - 1) * SCROLL_COLS, screenTrackerDown + linesDown * SCROLL_COLS, SCROLL_COLS);
			if (linesDown > 0)
				screenTrackerDown = (char*)krealloc(screenTrackerDown, linesDown * SCROLL_COLS);
			else
				kfree(screenTrackerDown);
		}
		else
		{
			for (int i = 0; i < SCROLL_COLS; i += 2)		// empty last row
				screen[(ROWS - 1) * SCROLL_COLS + i] = 0;
		}
	}
}

char* getTrackerUp()
{
	return screenTrackerUp;
}

char* getTrackerDown()
{
	return screenTrackerDown;
}