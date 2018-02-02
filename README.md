ZeeValidation
===============

Once you have the corresponding data and MC trees you can produce plots following the instructions below (no CMSSW is needed here).

```
git clone https://github.com/kmondal/plotZeeValidation
cd plotZeeValidation
root -l -b
.L plotter.C++
plotter(data_file_name, mc_file_name, IDMVA_transformation_file)
hadd out_combined.root out_data.root out_zee.root
python combinePlots.py out_combined.root <Output_dir_for_plots_to_save>
```

This should create a .root file with all the plots. The plots (and the event categorization) are defined in the following files:

    plotvariables.dat (histogram definition)
    categories.dat (category definition)
    selection.dat (event selection definition as a ROOT TCut string)

The final python script combines data and MC histos and put them together with the appropriate style.

Deriving Shower Shape Corrections
=======================================

In order to derive shower shape transformation you can use makeTransformation.py script. Once the Zee validation ntuples have been produced you can 
run the script three times with different parameters to: 
1. Produce the .root file with the necessary plots 
```
python makeTransformation.py -p mc_ntuple_name.root data_ntuple_name.root 
```

2. Derive the transformations 
```
python makeTransformation.py -c
```

3. Test the transformations on the original ntuples and produce the correction plots
```
python makeTransformation.py -t mc_ntuple_name.root data_ntuple_name.root
```

IDMVA with systematic plot
==========================


The necessary histograms used as inputs are produced in the ZeeValidation step. More generally you need to add there all the plots for nominal, up, down variations which you want to plot. At this point you need to modify the david_template.py file in order to able to read the name of these plots. Then:

```
python morphbands.py david_template.py yourROOTFile.root
```

The output is another .root file with plots nomeplot_bottom nomeplot_top of the corrected histograms. Finally with the idmva_syst.py macro the systematic band is added to the distribution.

```
python idmva_syst.py --data your_combinedROOTFile.root --mc output_of_previous_step.root
```

In order to apply the additional linearly increasing systematic you can use the linearCorrection.py script (available in the same directory). Beware you need to run it BEFORE the morphbands step described above. To run it just choose the parameter of the linear correction (xmin, xmax, ymin, ymax) and update the input filename.