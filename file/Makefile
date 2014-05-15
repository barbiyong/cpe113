# Make File
#
#	Date : 2 April 2014
#
# Author : Aj.Sally
#	Modified : Phasathorn Suwansri (Lukkid)
#						 ID 56070503424
#
#  - Edited executables files
#  - Edited .c files and .h files
#  - Use @ command to hide message
#  - Add echo to show the status

ifeq ($(OSTYPE),WINDOWS)
	EXECEXT =.exe
	COMP	=__MINGCC__
	PLATFORM	=mingw
else
	EXECEXT =
	COMP	=__GCC__
	PLATFORM	=linux
endif

CCW = @gcc -Wall -Wextra

EXECUTABLES= projectSchedule$(EXECEXT) 

all : $(EXECUTABLES)

calculateDate.o : calculateDate.c calculateDate.h
	@gcc -c calculateDate.c
	@echo "Compiling calculateDate..."

developerManager.o : developerManager.c developerManager.h
	@gcc -c developerManager.c
	@echo "Compiling developerManager..."

linkedListQueue.o : linkedListQueue.c abstractQueue.h
	@gcc -c linkedListQueue.c
	@echo "Compiling linkedListQueue..."

linkedListNetwork.o : linkedListNetwork.c abstractNetwork.h
	@gcc -c linkedListNetwork.c
	@echo "Compiling linkedListNetwork..."

validateFunctions.o : validateFunctions.c validateFunctions.h structure.h
	@gcc -c validateFunctions.c
	@echo "Compiling validateFunctions..."

taskManager.o : taskManager.c taskManager.h structure.h calculateDate.h
	@gcc -c taskManager.c
	@echo "Compiling taskManager..."

projectSchedule.o : projectSchedule.c taskManager.h structure.h
	@gcc -c projectSchedule.c
	@echo "Compiling projectSchedule..."

dataManager.o : dataManager.c  abstractNetwork.h structure.h
	@gcc -c dataManager.c
	@echo "Compiling dataManager..."

projectSchedule$(EXECEXT) : projectSchedule.o taskManager.o validateFunctions.o linkedListNetwork.o linkedListQueue.o calculateDate.o developerManager.o dataManager.o
	$(CCW) -o projectSchedule$(EXECEXT) projectSchedule.o developerManager.o taskManager.o validateFunctions.o linkedListNetwork.o  linkedListQueue.o dataManager.o calculateDate.o
	@-rm *.o
	@echo "Successfull..."

clean : 
	@-rm *.o
	@-rm $(EXECUTABLES) 
	@echo "All .o files and .exe file have been deleted!!"