##
# Ladder
#
OBJ=ladder

$(OBJ): $(OBJ).c
	$(CC) $< $(LIBS) -g -o $@

run:
	./$(OBJ)

clean:
	rm -rf $(OBJ)

# end
