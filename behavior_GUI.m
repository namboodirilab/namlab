function behavior_GUI

global s running actvAx saveDir

mainPath = 'C:\Users\namboodirilab\OneDrive - University of California, San Francisco\Behavioral_acquisition_and_analysis';
addpath(mainPath)
saveDir = [mainPath '\data\'];          % where to save data


% Make figure
sz = get(0, 'ScreenSize'); % screen size
x = mean(sz([1, 3])); % center position
y = mean(sz([2, 4])); % center position
width = 1700;
height = 1000;

fig = uifigure('Position', [x - width/2, y - height/2, width, height]);


csdata ={'Number of trials', 25, 25, 50, 0;
        'Frequency(kHz)', 12, 3, 5, 0;
        'Predicted solenoid', '5+3', '5+3', '1+3', '1+3';
        'Probability of solenoid', '0+100', '0+100', '0+0', '0+0';
        'Solenoid open time (ms)', '3000+40', '3000+40', '0+30', '0+40';
        'Cue duration (ms)', 1000, 1000, 1000, 1000;
        'Delay to solenoid (ms)', '0+3000', '0+3000', '0+3000', '0+3000';
        'Pulse tone (1) or not (0)', 0, 0, 1, 0;
        'Speaker number', 1, 2, 2, 2;
        'Light number', 1, 2, 1, 2;
        'Go lick requirement', 0, 0, 0, 0;
        'Go lick tube (or solenoid)', 1, 1, 3, 1;
        'Sound(1), light(2)', 1, 1, 1, 1;
        'Ramp max delay', 5000, 5000,1200, 5000;
        'Ramp exponent', 1, 1, 1, 1;
        'Increasing cue (1) or not(0)', 0, 0, 0, 0;
        'Delay to deliver the second cue if there is one', 0, 0, 0, 0;
        'Second cue type: sound(1) light(2) nocue(0)', 0, 0, 0, 0;
        'Second cue frequency', 5, 5, 5, 5;
        'Second cue speaker number', 1, 1, 2, 2;
        'Second cue light number', 1, 1, 1, 1};
cscolnames = {'Variables', 'CS1', 'CS2', 'CS3', 'CS4'};
    
global csproperties    
csproperties = uitable(fig,'Data',csdata);
set(csproperties, 'columnname', cscolnames);
csproperties.Position(:) = [100 500 300 360];
csproperties.ColumnEditable = true;

lickdata = {'Number of licks required',  5, 5;
            'Fixed/variable check',      0, 0;
            'Predicted solenoid',        3, 3;
            'Probability of solenoid', 100, 0;
            'Solenoid open time (ms)',  30, 30;
            'Delay to solenoid (ms)',    0, 0;
            'Delay to next lick (ms)',100, 100;
            'Fixed/variable check',      0, 0;                
            'Min number of rewards',    100, 0;
            'Sound(1), light(2) or both(3)' 1 1;
            'Pulse tone (1) or not (0)' 0 1;
            'Sound Frequency (kHz)' 12 3;
            'Sound Duration (ms)' 0 1000;
            'Speaker number' 1 2;
            'Light number' 1 2;
            'Fixed side check', 0, 0};
lickcolnames = {'Variables', 'Lick1', 'Lick2'};      
global lickproperties 
lickproperties = uitable(fig, 'Data', lickdata);
set(lickproperties, 'columnname', lickcolnames);
csproperties.Position(:) = [500 500 200 300];
csproperties.ColumnEditable = true;




