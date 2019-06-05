TARGET = simulator
OBJS += arrival_process_dist.o
OBJS += distribution_det.o
OBJS += distribution_exp.o
OBJS += distribution_sum.o
OBJS += distribution_mix.o
OBJS += perf_model_service_rate.o
OBJS += perf_model_dist.o
OBJS += random_helper.o
OBJS += request_generator_dist.o
OBJS += request_stream.o
OBJS += request_stream_open.o
OBJS += request_stream_closed.o
OBJS += request_handler.o
OBJS += request_handler_dispatcher.o
OBJS += request_handler_dispatcher_join_shortest_queue.o
OBJS += request_handler_dispatcher_least_work_left.o
OBJS += request_handler_dispatcher_random.o
OBJS += request_handler_dispatcher_round_robin.o
OBJS += request_handler_central_queue_k.o
#OBJS += request_handler_fb_queue.o
OBJS += request_handler_fifo_queue.o
OBJS += request_handler_lcfs_queue.o
OBJS += request_handler_plcfs_queue.o
OBJS += request_handler_ps_queue.o
OBJS += request_handler_psjf_queue.o
OBJS += request_handler_random_queue.o
OBJS += request_handler_sjf_queue.o
OBJS += request_handler_srpt_queue.o
OBJS += request_handler_queueing_network.o
OBJS += stats_latency.o
OBJS += stats_tail_latency.o
OBJS += stats_throughput.o
OBJS += simulator.o
OBJS += run.o
OBJS += main.o
OBJS += json/jsoncpp.o
LIBS += -lm

CXX = g++ #clang++ #OSX
CXXFLAGS += -std=c++11 #-stdlib=libc++ #non-linux
CXXFLAGS += -MMD -MP # dependency tracking flags
CXXFLAGS += -I./
CXXFLAGS += -Wall -Werror -Wconversion
LDFLAGS += $(LIBS)
all: CXXFLAGS += -O2 # release flags
all: $(TARGET)

debug: CXXFLAGS += -ggdb  -D_GLIBCXX_DEBUG # debug flags
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

DEPS = $(OBJS:%.o=%.d)
-include $(DEPS)

clean:
	-@rm $(TARGET) $(OBJS) $(DEPS) handin.tar.gz 2> /dev/null || true
	@make --no-print-directory -C tests clean

handin: clean
	@tar cvzf handin.tar.gz *
	@echo "***"
	@echo "*** SUCCESS: Created handin.tar.gz"
	@echo "***"
