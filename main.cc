#include "cpu.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		ERR("need program");
		abort();
	}

	auto program = read_file(argv[1]);

	CPU cpu{};
	cpu.load_program(program);
	cpu.run();

	return 0;
}