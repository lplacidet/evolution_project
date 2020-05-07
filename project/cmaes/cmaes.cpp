

struct cmaes{
    int N;
    int mu;
    int lambda;
    float tau;
    float tau_c;
    float tau_sigma;
    Bird* population;
    Bird* offspring;
    float* C;
    float* s;
    float* s_sigma;
    float sigma;
    float* E;
    float* W;

    init(int N_, int mu_, int lambda_, float tau_, float tau_sigma_){
    	N = N_;
    	mu = mu_
    	lambda = lambda_;
    	tau = tau_;
    	tau_sigma = tau_sigma_;

    	x = Bird();
	    population = new Bird[mu];
	    for(int i=0; i<mu; i++){population[i] = x;}
	    offspring = new Bird[lambda];
	    C = eye(N);
	    s = new float[N];
	    s_σ = new float[N];
	    σ = 1.0;
	    E = new float[lambda*N];
	    W = new float[lambda*N];
    }
}