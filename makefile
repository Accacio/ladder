##
# Ladder
#
OBJ=ladder

-include $(OBJ).d
$(OBJ): $(OBJ).c
	$(CC) $< $(LIBS) -g -MMD -o $@

run:
	./$(OBJ)

clean:
	rm -rf $(OBJ) $(OBJ).d

# end
