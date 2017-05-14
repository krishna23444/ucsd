#This makefile contains the rules
# to make the MACEDON extra tools.

TOOLS-ALL = tools
CFLAGS = -DGCC3 -I../lib -g
LIBFLAGS = -L../lib -lmace

.PHONY : evaluate_cc

PROGS=monitor churn_schedule deviation hextoip routes-chord check-pastry fraction_deviation random_tree

tools: $(PROGS) 
#evaluate_cc

deviation: deviation.cc 
	g++ -o deviation deviation.cc $(CFLAGS) $(LIBFLAGS)
fraction_deviation: fraction_deviation.cc 
	g++ -o fraction_deviation fraction_deviation.cc $(CFLAGS) $(LIBFLAGS)
hextoip: hextoip.cc
	g++ $(CFLAGS) -g -o hextoip hextoip.cc

random_tree: random_tree.cc 
	g++ $(CFLAGS) -g -o random_tree random_tree.cc 

churn_schedule: churn_schedule.cc 
	g++ $(CFLAGS) -g -o churn_schedule churn_schedule.cc 

monitor: monitor.cc 
	g++ $(CFLAGS) -g -o monitor monitor.cc 

routes-chord: routes-chord.cc
	g++ $(CFLAGS) -g -o routes-chord routes-chord.cc


check-pastry: check-pastry.cc
	g++ $(CFLAGS) -g -o check-pastry check-pastry.cc

evaluate_cc: 
	$(MAKE) -C evaluate_cc

clean:
	rm -f $(PROGS)
#	$(MAKE) -C evaluate_cc clean
