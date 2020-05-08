#ifndef BESTIAU_HPP
#define BESTIAU_HPP
#include <chrono>
#include <deque>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#ifdef DISPLAY
//#include <curses.h>

template <typename W> void display(const W &w, bool needClear = false);
#endif

typedef std::chrono::duration<int, std::milli> ms;

enum Color { green, red, blue };

struct World {
	struct Obstacle {
		Obstacle(double Y) : y(Y) {}
		Obstacle(double X, double Y) : x(X), y(Y) {}
		Obstacle(Color c, int X, int Y) : color(c), x(X), y(Y) {}
		Color color = blue;
		double x = 0;
		double y = 0;
	};

	struct Bestiau {
		double x = 0.1;
		double y = 0.5;
		double prevy = y;
		double vit = 0;
		double forces = 0;
		double mass = 1.0;
		bool vivant = true;
		void updatePosition(double dt) {
			prevy = y;
			vit += dt * forces / mass;
			y += vit * dt;
		}
	};

	const double H = 1;
	const double W = 5;
	const double epaisseurBordures = 0.06;
	const double largeurObstacles = 0.18;
	const double hauteurPassage = 0.25;
	double dist = 0;
	Bestiau bestiau;
	const double g = -1.5;
	const double force = 30;
	double gravity = 2;
	std::default_random_engine gen{0};

	double dt = 1.0 / 25.0;
	double vitesseDefilement = 0.5;

	double ecartCible = 4;
	double prochainObstacle = 0;

	std::deque<Obstacle> obstacles;
	bool up = false;

	void testCollision() {
		if (bestiau.y <= epaisseurBordures || bestiau.y >= H - epaisseurBordures) {
			bestiau.vivant = false;
		} else if (obstacles.size()) {
			const Obstacle &o = obstacles.front();
			if (o.x <= bestiau.x && o.x + largeurObstacles >= bestiau.x &&
			    (o.y > bestiau.y || o.y + hauteurPassage < bestiau.y)) {
				bestiau.vivant = false;
			}
		}
	}
	void bestiauUp() { up = true; }
	void update() {
		if (up) {
			bestiau.forces = force;
			up = false;
		} else {
			bestiau.forces = -gravity;
		}
		bestiau.updatePosition(dt);
		testCollision();
		double dep = vitesseDefilement * dt;
		dist += dep;
		prochainObstacle -= dep;
		if (prochainObstacle <= 0) {
			std::uniform_real_distribution<double> udist(0, H - hauteurPassage);
			obstacles.push_back(Obstacle(W + 0.1, udist(gen)));
			std::normal_distribution<double> ndist(ecartCible, ecartCible / 4.0);
			prochainObstacle = ndist(gen);
		}
		vitesseDefilement += 0.05 * dt;
		for (auto &o : obstacles) o.x -= dep;
		while (obstacles.size() && obstacles.front().x < (0.0 - largeurObstacles * 2.0))
			obstacles.pop_front();
	}
};


struct FlappyXP {
	static const constexpr int viewDist = 1;
 	// template <typename G> static G randomInit(size_t nbReguls = 0, int f = 0, int i = 0, int n = 0) {
	// 	G g(f, i, n);
	// 	g.randomParams();
	// 	// input
	// 	// bestiau's height
	// 	// next obstacle's height
	// 	// next obstacle's dist
	// 				// g.addRandomProtein(G::ProteinType_t::input, "h");
	// 				// g.addRandomProtein(G::ProteinType_t::input, "0h");
	// 				// g.addRandomProtein(G::ProteinType_t::input, "0d");
	// 	if (viewDist > 1) {
	// 					// g.addRandomProtein(G::ProteinType_t::input, "1h");
	// 					// g.addRandomProtein(G::ProteinType_t::input, "1d");
	// 	}
	// 	// two outputs, changing ratio = press up
	// 				// g.addRandomProtein(G::ProteinType_t::output, "0");
	// 				// g.addRandomProtein(G::ProteinType_t::output, "1");
	// 				// g.randomReguls(nbReguls);
	// 	return g;
	// }

	template <typename I> static void evaluate(I &ind, bool dbg = false) {
#ifdef DISPLAY
		initscr();
		timeout(0);
		noecho();
		curs_set(FALSE);
#endif
		const int NRUN = 3;
		auto &g = ind;
		double d = 0;
		for (int r = 0; r < NRUN; ++r) {
			World world;
			world.gen.seed(r * 100);
			while (world.bestiau.vivant) {
#ifdef DISPLAY
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				display(world, world.dist == 0);
#endif
				world.update();
				// update inputs
				float birdY = world.bestiau.y;
				float birdVY = world.bestiau.vit;
                float obsX;
                float obsY;
				if (world.obstacles.size() > 0) {
					const auto &o = world.obstacles.front();
					obsY = o.y + world.hauteurPassage * 0.5;
					obsX = o.x / world.W;
				} else {
					obsY = world.H * 0.5;
					obsX = 1000.0;
				}
				if (g.doFlap(birdY, birdVY, obsX, obsY)) world.bestiauUp();
			}
			d += world.dist;
		}
#ifdef DISPLAY
		endwin();
#endif
		return d;
	}
};

#endif

float test(float f){
    return f+1;
}

/*
template <typename I> static double evaluate_bird(I &ind, bool dbg = false) {
#ifdef DISPLAY
	initscr();
	timeout(0);
	noecho();
	curs_set(FALSE);
#endif
	const int NRUN = 3;
	auto &g = ind;
	double d = 0;
	for (int r = 0; r < NRUN; ++r) {
		World world;
		world.gen.seed(r * 100);
		while (world.bestiau.vivant) {
#ifdef DISPLAY
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            display(world, world.dist == 0);
#endif
            world.update();
            // update inputs
            float birdY = world.bestiau.y;
            float birdVY = world.bestiau.vit;
            float obsX;
            float obsY;
            if (world.obstacles.size() > 0) {
                const auto &o = world.obstacles.front();
                obsY = o.y + world.hauteurPassage * 0.5;
                obsX = o.x / world.W;
            } else {
                obsY = world.H * 0.5;
                obsX = 1000.0;
            }
            if (g.doFlap(birdY, birdVY, obsX, obsY)) world.bestiauUp();
        }
        d += world.dist;
    }
#ifdef DISPLAY
    endwin();
#endif
    return d;
}
#endif
*/