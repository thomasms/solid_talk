#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

struct Plot{
    Plot() : 
        _xlabel(""), _ylabel("")
    {}

    std::string getXLabel() const{
        std::cout << "Plot::getXLabel\n";
        return _xlabel;
    }    

    std::string getYLabel() const{
        std::cout << "Plot::getYLabel\n";
        return _ylabel;
    }    

    virtual void setXLabel(std::string label){
        std::cout << "Plot::setXLabel\n";
        _xlabel = label;
    }    

    virtual void setYLabel(std::string label){
        std::cout << "Plot::setYLabel\n";
        _ylabel = label;
    }
    
    // other stuff here

  protected:
    std::string _xlabel;
    std::string _ylabel;
};

struct LinePlot : public Plot{
    LinePlot() : Plot() {}

    // other stuff here for line plotting
    // no problem here!
};

struct HistogramPlot : public Plot{
    HistogramPlot() : Plot() {
    }

    // other stuff here for histogram plotting
    // no problem here!
};

// this is not really a 'is-a' relationship
// maybe use composition instead
// or use private inheritance if you want 'has-a'
// relationship
struct NormalisedHistogramPlot : private HistogramPlot{
    NormalisedHistogramPlot() : HistogramPlot() {
        _ylabel = "normalised counts";
    }

    using HistogramPlot::setXLabel;
    using HistogramPlot::getXLabel;
    using HistogramPlot::getYLabel;
};

struct SpecialNormalisedHistogramPlot : public NormalisedHistogramPlot{
    SpecialNormalisedHistogramPlot() : NormalisedHistogramPlot() {
    }

    // other stuff
};

// test each type of plot
void testPlot(std::shared_ptr<Plot>& plot, std::string property)
{
    plot->setXLabel("Energy (eV)");
    plot->setYLabel(property);

    assert(plot->getXLabel() == "Energy (eV)");
    assert(plot->getYLabel() == property);
}

// test each type of plot
void testNormalisedPlot(std::shared_ptr<NormalisedHistogramPlot>& plot)
{
    plot->setXLabel("Energy (eV)");

    assert(plot->getXLabel() == "Energy (eV)");
    assert(plot->getYLabel() == "normalised counts");
}

// main program
int main(int argv, char** argc){

    std::vector<std::shared_ptr<Plot>> plots = {
        std::make_shared<Plot>(),
        std::make_shared<LinePlot>(),
        std::make_shared<HistogramPlot>(),
    };

    std::vector<std::shared_ptr<NormalisedHistogramPlot>> norm_plots = {
        std::make_shared<NormalisedHistogramPlot>(),
        std::make_shared<SpecialNormalisedHistogramPlot>()
    };


    const std::vector<std::string> test_props = {
        "Cross section (barns)",
        "alpha (-)",
        "Heating (kw)"
    };

    try{
        for(auto prop: test_props){
            for(auto& plot: plots){
                testPlot(plot, prop);
            }
        }
        for(auto& plot: norm_plots){
            testNormalisedPlot(plot);
        }
    }
    catch(std::exception& ex){
        std::cout << "We got an error => " << ex.what() << "\n";
    }

    return 0;
}
