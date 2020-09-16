CC=g++
OBJECT=parser #生成的目标文件
FILE=../test/gcd.c #路径
$(OBJECT): util.h
	$(CC) -o $(OBJECT) *.cpp
	@./$(OBJECT) $(FILE) #编译后立刻运行
clean:
	@rm -f $(OBJECT)  *.o