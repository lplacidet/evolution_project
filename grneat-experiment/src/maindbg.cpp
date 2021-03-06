//#define OMP
#define DISPLAY
#include <chrono>
#include "coverage.hpp"
#include "include/gaga/gaga.hpp"
#include "include/grgen/multiint.hpp"
#include "include/grgen/real.hpp"
#include "include/grgen/grn.hpp"
#include "signalxp.hpp"
#include "coverage.hpp"
#include "bestiau.hpp"
// #include "ship.hpp"

int main(int argc, char** argv) {
	using GRN_t = GRN<RealC>;

	// using XP = DoubleFreqXP;
	// using XP = LowPassXP;
	using XP = FlappyGRNXP;
	// using XP = CoverageXP;
	// using XP = ShipEscape::shipXP;

	if (argc > 1) {
		std::ifstream t(argv[1]);
		std::stringstream buffer;
		buffer << t.rdbuf();
		GRN_t g(buffer.str(), 0, 1, 0);
		GAGA::Individual<GRN_t> ind(g);

		float g1 = stof(argv[2]);
		float g2 = stof(argv[3]);
		float g3 = stof(argv[4]);
		float g4 = stof(argv[5]);
		float g5 = stof(argv[6]);

		XP::evaluate_genes(g1,g2,g3,g4,g5, true);
		std::cerr << "Fitnesses : " << std::endl;
		for (auto& f : ind.fitnesses) {
			std::cerr << " - " << f.first << " : " << f.second << std::endl;
		}
	}
	return 0;
}
