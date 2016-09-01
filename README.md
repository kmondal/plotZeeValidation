- Do plotting as described here: https://github.com/matteosan1/flashgg/blob/master/Validation/Readme.md#zeevalidation

```
cd $CMSSW_BASE/..
git clone https://github.com/flashgg-validation/plotZeeValidation
cd plotZeeValidation
```

(I have put my changes in this repository as the code from the repository in the original recipe does not work out of the box for me)

- Creating data vs. MC plots:
  ```
  root -l
  .L plotter.C++
  ```
  then
     ```
     plotter("$CMSSW_BASE/src/flashgg/Validation/output/output_SingleElectron.root", "$CMSSW_BASE/src/flashgg/Validation/output/output_mc_single.root")
     ```
  - combine the two output files:
     ```
     hadd -f out_combined.root out_zee.root out_data.root
     ```
  - produce plots:
    ```
    python combinePlots.py out_combined.root
    ```
- To derive corrections for certain variables, one can do:
```
 python makeTransformation.py -p \
          $CMSSW_BASE/src/flashgg/Validation/output/output_mc_single.root \
          $CMSSW_BASE/src/flashgg/Validation/output/output_SingleElectron.root
```
- Derive the MC to data corrections:
```
python makeTransformation.py -c
```
- Test the corrections:
```
python makeTransformation.py -t \
          $CMSSW_BASE/src/flashgg/Validation/output/output_mc_single.root \
          $CMSSW_BASE/src/flashgg/Validation/output/output_SingleElectron.root
```