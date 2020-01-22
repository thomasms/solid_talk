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

struct NormalisedHistogramPlot : public HistogramPlot{
    NormalisedHistogramPlot() : HistogramPlot() {
        _ylabel = "normalised counts";
    }

    virtual void setYLabel(std::string label) override{
        // problem: Throws exception 
        // since normalised histogram y label (in this case)
        // has a fixed ylabel 
        std::cout << "NormalisedHistogramPlot::setYLabel\n";
        throw std::runtime_error("NormalisedHistogramPlot::setYLabel - Should not be called!");
    }

    // other stuff here
};

// test each type of plot
void testPlot(std::shared_ptr<Plot>& plot, std::string property)
{
    plot->setXLabel("Energy (eV)");
    plot->setYLabel(property);

    assert(plot->getXLabel() == "Energy (eV)");
    assert(plot->getYLabel() == property);
}

// main program
int main(int argv, char** argc){

    std::vector<std::shared_ptr<Plot>> plots = {
        std::make_shared<Plot>(),
        std::make_shared<LinePlot>(),
        std::make_shared<HistogramPlot>(),
        std::make_shared<NormalisedHistogramPlot>()
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
    }
    catch(std::exception& ex){
        std::cout << "We got an error => " << ex.what() << "\n";
    }

    return 0;
}
