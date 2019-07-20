Hello!

Welcome to LC3-Sim v0.1.0

Right now this program is in Beta.  There are many bugs, many things that aren't implemented, and somethings are implemented incorrectly, or worse partially correctly.

In light of this, I beg that you heed this warning:

Do not trust this simulator!

I can guarentee that the assembler won't break your file, but I can't guarentee that the assembler is fully functional.  If you must save, it can be done by typing 

	'save x<starting_address> x<ending_address>'

into the console, and it will try to save the file.
I can't guarentee that the save will work.  It probably will, but I beg that you make your adjustments eitherwith great care or somewhere that has reputable saving features, like notepad.exe or any word processor.

Known Dangers:

	 Don't undo with input.  It will likely crash.

Bugs:

	 There are going to be bugs.  I apologize beforehand for any time lost, but know that every bug you find forme is a bug someone won't suffer in a future version.

	 If you are able to find a bug, I ask that you first check that you have the newest version of the simulator.you can reach me at jbmelberg@eiu.edu
Please give me a description of what you were doing, what happened, and the code that you were using. I can't promise that I will be able to fix it that day, but I plan on bugfixing on a weekly basis.

If you would like to avoid the bugs, just follow these tips:

	Use a different simulator.  This program is in Beta, so there are bugs that you can see and bugs you can't see.

	Use the interface slowly, as not every button is shut off when it should be.

	Check the performance against a different simulator.

Suggestions:


 If you think of a feature which you would like to see added, please let me know.  I have much planned for this simulator, and your input is definitely appreciated.


Things to do and see:

	 Down in the bottom right corner, there is a box that says "<empty>".  This is one of the most useful features I have ever implemented.
That thing lets you run and unrun code!  It is sorta buggy, like the rest of the program, but it is something that I sorely miss whenever I code some other language.  Treat it gently, and it will likely serve you well.


****************** Memory and Stack ******************

The memory views and stack view offer insight into the state of the machine.  For LC3 info, please address the included Appendix.

Each line has the following:

	A Break point (BR), which can be toggled on or off. Break points that are on will halt the execution of code running in the simulator.
	
	An Address (Addr).  This is the address of the line.

	A Name (Name).  This is the label of the line.  Set it to allow easy manuvering through the code, as well as for good code writing practices.

	A Value (Value).  This is the value set to that address.

	A Mnemonic (Mnem).  This describes how the system will interact with the Value during computation.  To set this, change the line's value.

	A Comment (Comments).  Use this to keep track of a line's purpose.

Special Features:

	The Stack is an inverted view of the Memory Space, starting at the beginning of the USER SPACE (see appendix).

	There are input bars for each of the views.  Enter a Label or line value and hit 'GOTO'. If it exists and is within range, that view will jump to that spot.

	Double clicking the PC or SP will cause the view to follow any changes to that register's value.  PC follows PR, SP follows R6.



****************** Registers and Devices ****************** 

For all LC3 specific questions, please see the included Appendix.

To the left of the black rectangle (the screen) is a set of values.  These are the registers. Holding the mouse over each of them will result in a tool tip on what they are expected to be.

To the right of the screen, there are 4 buttons.  

	Continue.  Continue lets the system run until it hits a break point or halt.

	Halt.  Halt... halts the system.

	Next.  Next runs until the PC is at a value one higher than what is was when the button was pressed, or until a breakpoint is hit.

	Step.  Step executes one line.

Below the screen is a marker that will display the mouses position on the display.

Above the screen is a set of 4 buttons.  Those marked TBD do nothing, and are slated for future use.
	 See Tool Box, History  for info on Undo and Redo.

	The display can be cleared using the menu at the top of the application.


****************** Tool Box ****************** 

The Tool Box is the set of tabs in the lower left hand corner.

	Console has only one function at the moment.  It is useful for saving.

		To save, type 'save x{the beginning address, base 16} x{the ending address, inclusive, base 16}'.  You will be prompted for which file you would like to save to.

	Watches is a set of lines for easy viewing specific parts of memory.  To change the line that a watch points at, edit it's address (Addr).

	History is a very powerful tool at your disposal.  Almost all actions in the simulator that have to do with LC3 are recorded here.  To go to a state, click it.  The previously mentioned Undo and Redo traverse this stack up and down, respectively.

	Input is the i/o of the system.  To use, click the grey box and type.  The system will take your key presses as input.

****************** Contact ****************** 

If you have an issue, see a bug, would like to suggest a feature, etc, please do not hesitate to contact me at jbmelberg@eiu.edu.

Thank you so much for trying my program, your feedback is greatly valued.


















