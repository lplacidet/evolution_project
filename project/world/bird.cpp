struct Bird {
	static const nbgenes = 42;
	float m_fitness;
	bool m_evaluated = false;
	float[nbgenes] m_genes;

	Bird(){
		// Random genes generation
	}

	explicit Bird(float* genes) {
		m_genes = genes;
	}

	void mutate(){
	}

	static void crossover(Bird b1, Bird b2){
		float[] newgenes = new float[nbgenes];
		for(int i=0; i<nbgenes; i++){
			newgenes[i] = Random.random() < 0.5 ? b1.m_genes[i] : b2.m_genes[i];
		}
		return Bird(newgenes);
	}

}