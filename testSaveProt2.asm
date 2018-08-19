	.ORIG x3000
START 
    LD  R6, TOP 	; initialize stack pointer
    AND R5, R5, #0	; frame pointer = null
	
    LEA R0, P 	; i = 0
    LEA R1, Q 	
	
LOOP0 
    LEA R2, P_END 	; while (corner points remain)
    NOT R2, R2	;
    ADD R2, R2, #1	;
    ADD R2, R0, R2	; 
     BRz ELOOP0 	
	
    LDR R3, R0, #0	;  
    ADD R6, R6, #-1	;
    STR R3, R6, #0	;   push P[i].row
	
    LDR R3, R0, #1	;
    ADD R6, R6, #-1	;
    STR R3, R6, #0	;   push P[i].column
	
    LDR R3, R1, #0	;
    ADD R6, R6, #-1	;
    STR R3, R6, #0	;   push Q[i].row
	
    LDR R3, R1, #1	;
    ADD R6, R6, #-1	;
    STR R3, R6, #0	;   push Q[i].column
	
    LD  R3, RED 	;
    ADD R6, R6, #-1	;
    STR R3, R6, #0	;   push RED
	
    JSR DRAW_RECT 	;   draw_rect(P[i].row, P[i].column,
    ADD R6, R6, #5	;             Q[i].row, Q[i].column, RED)
	
    ADD R0, R0, #2	;
    ADD R1, R1, #2	;   i = i + 1
     BR LOOP0 	; end while
	
ELOOP0 
    LD  R0, F_ROW 	;push start row
    ADD R6, R6, #-1	;
    STR R0, R6, #0	;		
	
    LD  R0, F_COL 	;
    ADD R6, R6, #-1	;
    STR R0, R6, #0	;push start column
	
    LD  R0, RED 	;
    ADD R6, R6, #-1	;
    STR R0, R6, #0	;push red
	
	
    LD  R0, BLACK 	;
    ADD R6, R6, #-1	;
    STR R0, R6, #0	;push black
	
    JSR F_FILL 	
	
    LD  R0, WHITE 	;
    ADD R6, R6, #-1	;
    STR R0, R6, #0	;push red
	
STOP 
    TRAP  x0000	
	
TOP     xC000	; where does the stack begin?
RED     x7C00	; desired color for rectangles
WHITE     xFFFF	
BLACK     x0000	; the pixel color to be replaced
F_ROW     6	; where to start the flood fill...
F_COL     11	; currently (6,11)
	;; P stores the coordinates of the upper-left corner points
	
P     5	; (5, 10)
    10	;
    20	; (20, 40)
    40	;
    90	; (90, 90)
    90	;
    10	; (10, 60)
    60	;
    0	; (0, 0)
    0	
P_END     65535	
	;; Q stores the coordinates of the lower-right corner points
	
Q     30	; (30, 50)
    50	;
    100	; (100, 100)
    100	;
    95	; (95, 120)
    120	;
    70	; (70, 80)
    80	;
    123	; (123, 127)
    127	
Q_END     65535	
	;; Subroutine ................................................................
	
	;; Draw a rectangle, given the coordinates of opposing corner points
	;;
	;; Parameters:
	;; 	ULrow   (upper left corner)
	;; 	ULcol
	;; 	LRrow   (lower right corner)
	;; 	LRcol
	;; 	color   (desired color)
	;; 
	;; Frame offsets:
	;; 	-4  saved R4
	;; 	-3  saved R3
	;; 	-2  saved R2
	;; 	-1  saved R1
	;; 	 0  saved R0
	;; 	+1  previous frame pointer
	;; 	+2  return address
	;; 	+3  color
	;; 	+4  LRcolumn
	;; 	+5  LRrow
	;; 	+6  ULcolumn
	;; 	+7  ULrow
	
	;; PROLOG
	
DRAW_RECT 
    ADD R6, R6, #-1	;
    STR R7, R6, #0	; push return address
	
    ADD R6, R6, #-1	;	
    STR R5, R6, #0	; push previous frame pointer
	
    ADD R5, R6, #-1	; establish frame pointer for this frame
	
    STR R0, R6, #-1	; save registers being used in this routine
    STR R1, R6, #-2	;
    STR R2, R6, #-3	;
    STR R3, R6, #-4	
    STR R4, R6, #-5	
    ADD R6, R6, #-5	; adjust stack pointer to the top of the frame
	;; BODY
	;; Draw horizontal line segments 
	
    LDR R3, R5, #7	; R3 = ULrow
    LDR R4, R5, #5	; R4 = LRrow
	
    LDR R0, R5, #6	; c = ULcolumn
	
LOOP1 
    LDR R1, R5, #4	; while (c <= LRcolumn)
    NOT R2, R0	;
    ADD R2, R2, #1	;
    ADD R1, R1, R2	;
     BRn ELOOP1 	;
	
BODY1 
    ADD R6, R6, #-1	;   
    STR R3, R6, #0	;    push ULrow
	
    ADD R6, R6, #-1	;
    STR R0, R6, #0	;    push c
	
    LDR R1, R5, #3	;
    ADD R6, R6, #-1	;
    STR R1, R6, #0	;    push color 
	
    JSR DRAW_PIXEL 	;    draw_pixel(ULrow, c, color)
    ADD R6, R6, #3	;    remove parameters
	
    ADD R6, R6, #-1	;    
    STR R4, R6, #0	;    push LRrow
	
    ADD R6, R6, #-1	;
    STR R0, R6, #0	;    push c 
	
    ADD R6, R6, #-1	;
    STR R1, R6, #0	;    push color
	
    JSR DRAW_PIXEL 	;    draw_pixel(LRrow, c, color)
    ADD R6, R6, #3	;
	
    ADD R0, R0, #1	;    c = c + 1
     BR LOOP1 	; end while
	;; Draw vertical line segments 
	
ELOOP1 
    LDR R3, R5, #6	; R3 = ULcolumn
    LDR R4, R5, #4	; R4 = LRcolumn
	
    LDR R0, R5, #7	; r = ULrow
LOOP2 
    LDR R1, R5, #5	; while (r <= LRrow)
    NOT R2, R0	;
    ADD R2, R2, #1	;
    ADD R1, R1, R2	;
     BRn ELOOP2 	;
	
BODY2 
    ADD R6, R6, #-1	;   
    STR R0, R6, #0	;    push r
	
    ADD R6, R6, #-1	;
    STR R3, R6, #0	;    push ULcolumn
	
    LDR R1, R5, #3	;
    ADD R6, R6, #-1	;
    STR R1, R6, #0	;    push color
	
    JSR DRAW_PIXEL 	;    draw_pixel(r, ULcolumn, color)
    ADD R6, R6, #3	;    remove parameters
	
    ADD R6, R6, #-1	;    
    STR R0, R6, #0	;    push r
	
    ADD R6, R6, #-1	;
    STR R4, R6, #0	;    push LRcolumn
	
    ADD R6, R6, #-1	;
    STR R1, R6, #0	;    push color
	
    JSR DRAW_PIXEL 	;    draw_pixel(r, LRcolumn, color)
    ADD R6, R6, #3	;
	
    ADD R0, R0, #1	;    r = r + 1
     BR LOOP2 	; end while
	;; EPILOG
	
ELOOP2 
    LDR R4, R5, #-4	
    LDR R3, R5, #-3	
    LDR R2, R5, #-2	; restore R2
    LDR R1, R5, #-1	; restore R1
    LDR R0, R5, #0	; restore R0
    LDR R7, R5, #2	; get the return address
    LDR R5, R5, #1	; restore the previous frame pointer
    ADD R6, R6, #7	; adjust the stack pointer, deallocate frame
    RET 	
	;; Subroutine ................................................................
	
	;; Draw a pixel on the graphics display unit at a specified location
	;;
	;; Parameters:
	;; 	row     -- a value between 0 and 123
	;; 	column  -- a value between 0 and 127
	;; 	color   -- a 15-bit RGB value
	;;
	;; Frame offsets:
	;; 	-4  saved R4
	;; 	-3  saved R3
	;; 	-2  saved R2
	;; 	-1  saved R1
	;; 	 0  saved R0
	;; 	+1  previous frame pointer
	;; 	+2  return address
	;; 	+3  color
	;; 	+4  column
	;; 	+5  row
	
	;; PROLOG
	
DRAW_PIXEL 
    ADD R6, R6, #-1	;
    STR R7, R6, #0	; push return address
	
    ADD R6, R6, #-1	;	
    STR R5, R6, #0	; push previous frame pointer
	
    ADD R5, R6, #-1	; establish frame pointer for this frame
	
    STR R0, R6, #-1	; save registers being used in this routine
    STR R1, R6, #-2	;
    STR R2, R6, #-3	;
    STR R3, R6, #-4	;
    STR R4, R6, #-5	;
    ADD R6, R6, #-5	; adjust stack pointer to the top of the frame
	;; BODY
	
    LDR R0, R5, #5	; R0 = row
    LDR R1, R5, #4	; R1 = column
    LDR R2, R5, #3	; R2 = color
	
    ADD R3, R0, #0	;
    LD  R4, PIX_PER_ROW 	;
    MUL R3, R3, R4	;
    ADD R3, R3, R1	; R3 = PIX_PER_ROW * row + column
	
    LD  R4, VIDEO_ADDR 	
    ADD R4, R4, R3	; R4 = address of desired pixel
	
    STR R2, R4, #0	; Video[row,column] = color
	;; EPILOG
	
    LDR R4, R5, #-4	; restore R4
    LDR R3, R5, #-3	; restore R3
    LDR R2, R5, #-2	; restore R2
    LDR R1, R5, #-1	; restore R1
    LDR R0, R5, #0	; restore R0
    LDR R7, R5, #2	; get the return address
    LDR R5, R5, #1	; restore the previous frame pointer
    ADD R6, R6, #7	; adjust the stack pointer, deallocate frame
    RET 	
	;; Constants used by DRAW_PIXEL
	
VIDEO_ADDR     xC000	; base address of the video display
PIX_PER_ROW     128	; number of pixels per row
	
	;; Subroutine .............................................................
	
	;; Fills an area filled by a color with a second color started a pixel
	;;
	;; Parameters:
	;; row		-- a value between 0 and 123
	;; column	-- a value between 0 and 127
	;; color2	-- a 15 bit RGB value, the replaceing color
	;; color1	-- a 15 bit RGB value, the color to be replaced
	;; 
	;; Frame offsets
	;;	-1 	saved R1
	;; 	 0  saved R0
	;; 	+1  previous frame pointer
	;; 	+2  return address
	;; 	+3  color1
	;;	+4	color2
	;; 	+5  column
	;; 	+6  row
	
	
	
	;;local frame offsets
	;;
	;;
	;;  
	;;	 0 color 1
	;;	 1 color 2
	;;	 2 column
	;;   3 row
	;;
	
	;; PROLOG ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
F_FILL 
    ADD R6, R6, #-1	;
    STR R7, R6, #0	; push return address
	
    ADD R6, R6, #-1	;	
    STR R5, R6, #0	; push previous frame pointer
	
    ADD R5, R6, #-1	; establish frame pointer for this frame
    STR R0, R6, #-1	; save registers
    STR R1, R6, #-2	
    ADD R6, R6, #-2	;move up pointer;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	;; BODY ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;
	;; The way this program works is by first loading a local stack with the 
	;; row and column and the color that we will eventually replace the 
	;; current pixel with.  It then tests the current pixel against that color
	;; returning zero if they are equal, a non-zero if they aren't.
	;;
	;; If the colors are the same, push the correct color onto the stack then
	;; put the replacable color onto the stack aswell.
	;;
	;; Call F_FILL 4 times, 
	;;
	
	
	
	
	
	
	
	
	
	
	
	
    LDR R0, R5, #6	;
    ADD R6, R6, #-1	; store the row in the local stack
    STR R0, R6, #0	;
	
    LDR R0, R5, #5	;
    ADD R6, R6, #-1	; store the column in the local stack
    STR R0, R6, #0	;
	
    LDR R0, R5, #3	
    ADD R6, R6, #-1	; store the color to be replaced in the local stack
    STR R0, R6, #0	;
	
    JSR GET_PIXEL 	
    ADD R6, R6, #1	;poping the found pixel color
    LDR R0, R6, #-1	;and check it
	
	
	
     BRnp F_FILLCLEAN 	; there is a difference, so go back up
	; if you are here, the pixel you want to replace is the correct color
	
	
    LDR R0, R5, #4	;load replacing color
	
    ADD R6, R6, #-1	;push the intended color
    STR R0, R6, #0	; replace the top of the stack with the intended color
	
    JSR DRAW_PIXEL 	
	
    LDR R0, R5, #3	;get the color to be replaced
    ADD R6, R6, #-1	;	
    STR R0, R6, #0	;push it
	
	
	
    LDR R0, R6, #2	;These registers are actually not really needed for the
    LDR R1, R6, #3	;
    ADD R0, R0, #1	
    STR R0, R6, #2	
	;; EAST
	
	
    JSR F_FILL 	
	
    ADD R0, R0, #-2	
    STR R0, R6, #2	
	;; WEST
	
    JSR F_FILL 	
	
    ADD R0, R0, #1	
    STR R0, R6, #2	
	
	
    ADD R1, R1, #1	
    STR R1, R6, #3	
	;;SOUTH
	
    JSR F_FILL 	
	
    ADD R1, R1, #-2	
    STR R1, R6, #3	
	;;NORTH
	
	
    JSR F_FILL 	
    ADD R6, R6, #2	
	
	
	
	;; Epilog ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
F_FILLCLEAN 
    ADD R6, R6, #2	
    LDR R0, R5, #0	; restore R0
    LDR R1, R5, #-1	
    LDR R7, R5, #2	; get the return address
    LDR R5, R5, #1	; restore the previous frame pointer
    ADD R6, R6, #4	; adjust the stack pointer, deallocate frame
    RET 	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	
	
	
	
	;; Subroutine ................................................................
	
	;;
	;; Returns the 15 bit RGB value of the pixel
	;;
	;; Parameters:
	;; 	row     -- a value between 0 and 123
	;; 	column  -- a value between 0 and 127
	;;	color	-- the color found
	;;
	;; Frame Offsets
	;; 	-1  saved R1
	;; 	 0  saved R0
	;; 	+1  previous frame pointer
	;; 	+2  return address
	;;	+3	color
	;;	+4	column
	;;	+5	row
	;;	
	;;	Return Offsets
	;;	+3 color
	;;	
	;;
	
	
	;; PROLOG
	
GET_PIXEL 
    ADD R6, R6, #-1	;
    STR R7, R6, #0	; push return address
	
    ADD R6, R6, #-1	;	
    STR R5, R6, #0	; push previous frame pointer
	
    ADD R5, R6, #-1	; establish frame pointer for this frame
	
    STR R0, R6, #-1	; save registers being used in this routine
    STR R1, R6, #-2	;
	
    ADD R6, R6, #-2	;; end prolog
	
    LDR R0, R5, #5	;	get rows...	
    LD  R1, PIX_PER_ROW 	;
    MUL R1, R0, R1	;	... multiply it by pixel per row
    LDR R0, R5, #4	;	get column ....
    ADD R0, R0, R1	; R0 = PIX_PER_ROW * row + column
    LD  R1, VIDEO_ADDR 	
    ADD R0, R0, R1	
	
	
    LDR R0, R0, #0	;put the value at that location into R0
	
    LDR R1, R5, #3	;get the input color
    MUL R1, R1, #-1	;mult by -1
	
    ADD R0, R1, R0	;compare it
    STR R0, R5, #3	;store R0 into the stack
    LDR R1, R5, #-1	
    LDR R0, R5, #0	
    LDR R7, R5, #2	; get the return address
    LDR R5, R5, #1	; restore the previous frame pointer
    ADD R6, R6, #4	; adjust the stack pointer, deallocate frame
.END

