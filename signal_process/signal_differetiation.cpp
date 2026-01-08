class SignalDifferentiator {
    private:
        float prev_sample;
        float sampling_freq;
        bool first_input;
    public:
        SignalDifferentiator(float sampling_freq) : prev_sample(0), sampling_freq(sampling_freq), first_input(true){}
        float differentiateSample(float input){
            if(first_input){
                prev_sample = input;
                first_input = false;
                return 0;
            }
            float derivative = (input - prev_sample) * sampling_freq;
            prev_sample = input;
            return derivative;
        }

        void reset(){
            prev_sample = 0;
            first_input = true;
        }
};