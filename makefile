grep: index_vec.o re.o state_set.o state_vec.o str.o str_util.o subset.o

grep.o: re.h str.h str_util.h
index_vec.o: index_vec.h
re.o: re.h index_vec.h state_set.h state_vec.h str.h subset.h
state_set.o: state_set.h index_vec.h subset.h
state_vec.o: state_vec.h state.h
str.o: str.h
str_util.o: str_util.h str.h
subset.o: subset.h
test.o: re.h str.h str_util.h
