function behavior_GUI

global s running actvAx saveDir

mainPath = '/Users/mzhou/OneDrive - University of California, San Francisco';
addpath(mainPath)
saveDir = [mainPath '/data/'];          % where to save data


% Make figure
sz = get(0, 'ScreenSize'); % screen size
x = mean(sz([1, 3])); % center position
y = mean(sz([2, 4])); % center position
width = 1300;
height = 900;

fig = uifigure('Position', [x - width/2, y - height/2, width, height]);
fig.Units = 'normalized';


% Find available serial ports
availablePortslbl = uilabel(fig);
availablePortslbl.Text='Select Serial Port:';
availablePortslbl.Position = [10 850 100 24];
availablePorts = uidropdown(fig);
availablePorts.Position = [110 850 170 24];    %Make serial port drop down

serialInfo = instrhwinfo('serial');
port = serialInfo.AvailableSerialPorts;
if ~isempty(port)
    set(availablePorts,'Items',port)
end


%Connected to Field
connectlbl = uilabel(fig, 'Text', 'Connected to:', 'Position', [540 850 80 22]);
connectfield = uieditfield(fig,'text','Editable','off','Position', [620 850 120 22]);
filenamelbl = uilabel(fig, 'Text', 'Save file as:', 'Position', [540 820 70 22]);
filenamefield = uieditfield(fig,'text','Value','animalname','Editable','on','Position', [620 820 120 22]);
starttimelbl = uilabel(fig, 'Text', 'Start Time:', 'Position', [540 790 70 22]);
starttimefield = uieditfield(fig,'text','Editable','off','Position', [620 790 120 22]);

% Buttons for connecting or disconnecting to arduino
refreshbutton = uibutton(fig, 'Position',[220 790 100 40], 'Text','Refresh','FontSize',11, 'ButtonPushedFcn', {@pushRefresh,availablePorts});
connectbutton = uibutton(fig, 'Position',[10 790 100 40], 'Text','Connect','FontSize',11, 'Enable','off','ButtonPushedFcn', {@pushConnect,connectfield,availablePorts});
disconnectbutton = uibutton(fig, 'Position',[115 790 100 40], 'Text','Disconnect','FontSize',11, 'Enable','off','ButtonPushedFcn', {@pushDisconnect,connectbutton,refreshbutton,connectfield, availablePorts});

% Select experiment mode 
experiments = uipanel(fig, 'Title', 'Experiment modes:', 'FontSize',10,'Units','normalized', 'Position', [0.255 0.86 0.15 0.12]);
experimentmode = uidropdown('Parent',experiments,'Items',{'1: Cues with or without lick req','2: Random rewards','3: Lick for rewards','4: Decision making',...
    '5: Ramp timing task'}, 'ItemsData',[1 2 3 4 5 6], 'FontSize', 10);
experimentmode.Position = [20 50 170 24];
global selectedmode 
selectedmode = experimentmode.Value;

% uploadfield = uieditfield(fig,'text','Editable','off','Position', [100 170 150 22]);
uploadbutton = uibutton('Parent', experiments,'Text', 'Upload','FontSize', 10,...
    'Position', [60 18 80 24], 'ButtonPushedFcn', {@pushUpload,experimentmode,availablePorts,connectbutton});

% unittetx = uilabel(fig, 'Text', '* All time values are in units of ms', 'FontSize',11, 'Position', [570 830 190 30]);


% Data for csproperties and lick properties
csproperties ={'Number of trials', 25, 25, 50, 0;
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
cstable = uitable(fig,'Data',csproperties);
set(cstable, 'columnname', cscolnames);
cstable.FontSize = 9;
% csproperties.Position(:) = [50 500 400 300];
cstable.Units = 'normalized';
cstable.Position = [0.01 0.47 0.32 0.38];
cstable.ColumnEditable = [false true true true true];
assignin('base','csproperties',csproperties);

lickproperties = {'Number of licks required',  5, 5;
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
licktable = uitable(fig, 'Data', lickproperties);
set(licktable, 'columnname', lickcolnames);
licktable.FontSize = 9;
% lickproperties.Position(:) = [500 600 300 200];
licktable.Units = 'normalized';
licktable.Position = [0.34 0.47 0.22 0.38];
licktable.ColumnEditable = [false true true];
assignin('base','lickproperties', lickproperties);

% Make panels
% Optogenetics panel
Optopanel = uipanel(fig, 'Title','Optogenetics', 'FontSize',12,'FontWeight','bold',...
    'Units', 'normalized','Position',[0.01 0.255 0.18 0.2]);
randomlaser = uicheckbox('Parent', Optopanel, 'Text', 'Random laser?','FontSize', 10,'Position', [15 135 120 22]);
trialbytriallaser = uicheckbox('Parent', Optopanel, 'Text', 'Trial-by-tiral?', 'FontSize',10,'Position', [130 135 150 22]);
laserwrtcuetext = uilabel('Parent', Optopanel, 'Text', 'Laser latency wrt cue', 'FontSize', 10, 'Position', [15 115 100 22]);
laserwrtcue = uieditfield('Parent',Optopanel,'Editable','on','Position', [160 115 40 18]);
laserdurationtext = uilabel('Parent',Optopanel,'Text','Laser duration', 'FontSize',10, 'Position', [15 95 100 22]);
laserduration = uieditfield('Parent', Optopanel, 'Editable', 'on', 'Position',[160 95 40 18]);
laserpulseontext = uilabel('Parent',Optopanel,'Text','Laser pulse ON period', 'FontSize',10, 'Position', [15 75 130 22]);
laserpulseon = uieditfield('Parent', Optopanel, 'Editable', 'on', 'Position',[160 75 40 18]);
laserpulseofftext = uilabel('Parent',Optopanel,'Text','Laser pulse OFF period', 'FontSize',10, 'Position', [15 55 130 22]);
laserpulseoff = uieditfield('Parent', Optopanel, 'Editable', 'on', 'Position',[160 55 40 18]);
laserchecktext = uilabel('Parent',Optopanel, 'Text', 'Check laser or not', 'FontSize', 10, 'Position', [15 30 120 20]);
Rewardlasercheck = uicheckbox('Parent',Optopanel, 'Text', 'Reward', 'FontSize',10, 'Position',[140 30 70 18]);
CS1lasercheck = uicheckbox('Parent', Optopanel, 'Text', 'CS1', 'FontSize',10, 'Position', [15 10 50 18]);
CS2lasercheck = uicheckbox('Parent', Optopanel, 'Text', 'CS2', 'FontSize',10, 'Position', [70 10 50 18]);
CS3lasercheck = uicheckbox('Parent', Optopanel, 'Text', 'CS3', 'FontSize',10, 'Position', [125 10 50 18]);
CS4lasercheck = uicheckbox('Parent', Optopanel, 'Text', 'CS4', 'FontSize',10, 'Position', [180 10 50 18]);

% ITI panel
ITIpanel = uipanel(fig, 'Title', 'ITI', 'FontSize', 12, 'FontWeight','bold','Units', 'normalized',...
    'Position', [0.2 0.255 0.18 .2]);
Intervaldistributiontext = uilabel('Parent',ITIpanel,'Text','Interval distribution', 'FontSize',10, 'Position', [15 137 100 20]);
intervaldistribution = uidropdown('Parent',ITIpanel, 'Items', {'1. exponential ITI','2. uniform ITI', '3. poisson CS',...
    '4. poisson CS & Rw', '5. poisson Rw-CS'}, 'ItemsData',[1 2 3 4 5], 'FontSize', 10, 'Position', [120 135 110 20]);
maxdelaycuetovacuumtext = uilabel('Parent', ITIpanel, 'Text', 'max delay b/w cue and vacuum', 'FontSize', 10,'Position', [15 110 160 20]);
maxdelaycuetobacuum = uieditfield('Parent',ITIpanel,'Editable','on','Position', [180 110 40 18]);
meanITItext = uilabel('Parent', ITIpanel, 'Text', 'mean IIT (if same with maxITI,', 'FontSize', 10,'Position', [25 85 160 20]);
meanITItextline2 = uilabel('Parent', ITIpanel, 'Text', 'use fixed ITI)', 'FontSize', 10, 'Position', [55 75 100 20]);
meanITI = uieditfield('Parent',ITIpanel,'Editable','on','Position', [180 80 40 18]);
minITItext = uilabel('Parent', ITIpanel, 'Text', 'minITI', 'FontSize', 10, 'Position', [55 50 120 20]);
minITI = uieditfield('Parent',ITIpanel,'Editable','on','Position', [180 50 40 18]);
maxITItext = uilabel('Parent', ITIpanel, 'Text', 'maxITI truncation of ITI - ',...
    'FontSize', 10, 'Position', [25 25 180 20]);
maxITItextline2 = uilabel('Parent',ITIpanel, 'Text', 'min(maxITI, 3*meanITI)', 'FontSize', 10, 'Position', [25 15 140 20]);
maxITI = uieditfield('Parent',ITIpanel,'Editable','on','Position', [180 20 40 18]);

% Background rewards panel
bgdrpanel =  uipanel(fig, 'Title', 'Background rewards', 'FontSize', 12, 'FontWeight','bold',...
    'Units', 'normalized','Position', [0.39 0.255 0.18 0.2]);
bgdsolenoidtext = uilabel('Parent', bgdrpanel, 'Text', 'bgd solenoid #', 'FontSize', 10, 'Position', [15 135 100 18]);
bgdsolenoid = uieditfield('Parent', bgdrpanel, 'Editable','on', 'Position', [90 135 30 18]);
r_bgdtext = uilabel('Parent', bgdrpanel, 'Text', 'open time', 'FontSize', 10, 'Position', [135 135 50 18]);
r_bgd = uieditfield('Parent', bgdrpanel, 'Editable','on', 'Position', [190 135 30 18]);
bgdperiodtext = uilabel('Parent', bgdrpanel, 'Text','Background period T_bgd', 'FontSize',10, 'Position', [15 110 170 18]);
T_bgd = uieditfield('Parent', bgdrpanel, 'Position', [190 110 30 18]); 
mindelaybgdtocuetext = uilabel('Parent', bgdrpanel, 'Text','Min delay between bgd reward to cue', 'FontSize',10, 'Position', [15 85 170 18]);
mindelaybgdtocue = uieditfield('Parent', bgdrpanel, 'Position', [190 85 30 18]); 
mindelayfxdtobgdtext = uilabel('Parent', bgdrpanel, 'Text','Min delay between bgd', 'FontSize',10, 'Position', [25 65 140 18]);
mindelayfxdtobgdtextline2 = uilabel('Parent', bgdrpanel, 'Text',' and fxd reward', 'FontSize',10, 'Position', [35 55 100 18]);
mindelayfxdtobgd = uieditfield('Parent', bgdrpanel, 'Position', [190 60 30 18]); 
totPoisssolenoidtext = uilabel('Parent', bgdrpanel, 'Text','Total# background rewards', 'FontSize',10, 'Position', [15 35 160 18]);
totPoisssolenoid = uieditfield('Parent', bgdrpanel, 'Position', [190 35 30 18]); 
trialbytrialbgdsolenoidflag = uicheckbox('Parent',bgdrpanel, 'Text', 'Run trial-by-trial bgd rewards experiment?',...
    'FontSize', 10, 'Position', [15 10 250 18]);

% Make plot
ax = uiaxes(fig, 'Units','normalized','Position', [0.58 0.38 0.4 0.6]);
actvAx = ax;    % set as global so conditiong_prog can plot

lick1text = uilabel(fig, 'Text', 'Lick1s', 'FontColor', [0.2 0.6 1],'FontWeight','bold', 'FontSize', 12, 'Position', [750 155 40 20]);
lick1s = uieditfield(fig, 'Position', [795 150 60 30]);
lick2text = uilabel(fig, 'Text', 'Lick2s', 'FontColor', 0.65*[1, 1, 1],'FontWeight','bold', 'FontSize', 12, 'Position', [750 115 40 20]);
lick2s = uieditfield(fig, 'Position', [795 110 60 30]);
lick3text = uilabel(fig, 'Text', 'Lick3s', 'FontColor', [0.3 0 0],'FontWeight','bold', 'FontSize', 12, 'Position', [750 75 40 20]);
lick3s = uieditfield(fig, 'Position', [795 70 60 30]);
bgdsolenoidtext = uilabel(fig, 'Text', 'bgd solenoid', 'FontColor', 'k','FontWeight','bold', 'FontSize', 12, 'Position', [715 35 80 20]);
bgdsolenoids = uieditfield(fig, 'Position', [795 30 60 30]);

CS1soundtext = uilabel(fig, 'Text', 'CS1 sounds', 'FontColor', 'g','FontWeight','bold', 'FontSize', 12, 'Position', [865 155 80 20]);
CS1sounds = uieditfield(fig, 'Position', [940 150 60 30]);
CS2soundtext = uilabel(fig, 'Text', 'CS2 sounds', 'FontColor', 'r','FontWeight','bold', 'FontSize', 12, 'Position', [865 115 80 20]);
CS2sounds = uieditfield(fig, 'Position', [940 110 60 30]);
CS3soundtext = uilabel(fig, 'Text', 'CS3 sounds', 'FontColor', 'b','FontWeight','bold', 'FontSize', 12, 'Position', [865 75 80 20]);
CS3sounds = uieditfield(fig, 'Position', [940 70 60 30]);
CS4soundtext = uilabel(fig, 'Text', 'CS4 sounds', 'FontColor', [0.49 0.18 0.56],'FontWeight','bold', 'FontSize', 12, 'Position', [865 35 80 20]);
CS4sounds = uieditfield(fig, 'Position', [940 30 60 30]);

CS1lighttext = uilabel(fig, 'Text', 'CS1 lights', 'FontColor', [0 0.45 0.74], 'FontWeight','bold', 'FontSize', 12, 'Position', [1015 155 80 20]);
CS1lights = uieditfield(fig, 'Position', [1080 150 60 30]);
CS2lighttext = uilabel(fig, 'Text', 'CS2 lights', 'FontColor', [0.93 0.69 0.13], 'FontWeight','bold', 'FontSize', 12, 'Position', [1015 115 80 20]);
CS2lights = uieditfield(fig, 'Position', [1080 110 60 30]);
CS3lighttext = uilabel(fig, 'Text', 'CS3 lights', 'FontColor', [0.85 0.33 0.1], 'FontWeight','bold', 'FontSize', 12, 'Position', [1015 75 80 20]);
CS3lights = uieditfield(fig, 'Position', [1080 70 60 30]);
CS4lighttext = uilabel(fig, 'Text', 'CS4 lights', 'FontColor', [0.43 0.68 0.1],'FontWeight','bold', 'FontSize', 12, 'Position', [1015 35 80 20]);
CS4lights = uieditfield(fig, 'Position', [1080 30 60 30]);

solenoid1text = uilabel(fig, 'Text', 'Solenoid 1', 'FontColor', 'c','FontWeight','bold', 'FontSize', 12, 'Position', [1155 155 80 20]);
solenoid1s = uieditfield(fig, 'Position', [1220 150 60 30]);
solenoid2text = uilabel(fig, 'Text', 'Solenoid 2', 'FontColor', [0.64 0.08 0.18],'FontWeight','bold', 'FontSize', 12, 'Position', [1155 115 80 20]);
solenoid2s = uieditfield(fig, 'Position', [1220 110 60 30]);
solenoid3text = uilabel(fig, 'Text', 'Solenoid 3', 'FontColor', [1 0.5 0], 'FontWeight','bold', 'FontSize', 12, 'Position', [1155 75 80 20]);
solenoid3s = uieditfield(fig, 'Position', [1220 70 60 30]);
solenoid4text = uilabel(fig, 'Text', 'Solenoid 4', 'FontColor', [0.72 0.27 1],'FontWeight','bold', 'FontSize', 12, 'Position', [1155 35 80 20]);
solenoid5s = uieditfield(fig, 'Position', [1220 30 60 30]);
lickretractsolenoid1s = uilabel(fig, 'Text', 'Lick retract solenoid 1', 'FontColor', [0.3 0.75 0.93], 'FontWeight','bold', 'FontSize', 12, 'Position', [1090 235 150 20]);
lickretractsolenoid1text = uieditfield(fig, 'Position', [1220 235 60 30]);  
lickretractsolenoid2s = uilabel(fig, 'Text', 'Lick retract solenoid 2', 'FontColor',[0.97 0.28 0.18], 'FontWeight','bold', 'FontSize', 12, 'Position', [1090 195 150 20]);
lickretractsolenoid2text = uieditfield(fig, 'Position', [1220 195 60 30]);


% Test buttons 
testCS1 = uibutton(fig, 'Text', 'Test CS1', 'FontSize',11, 'Position',[30 180 100 40], 'ButtonPushedFcn', {@testCS1_fcn});
testCS2 = uibutton(fig, 'Text', 'Test CS2', 'FontSize',11, 'Position',[140 180 100 40], 'ButtonPushedFcn', {@testCS2_fcn});
testCS3 = uibutton(fig, 'Text', 'Test CS3', 'FontSize',11, 'Position',[250 180 100 40], 'ButtonPushedFcn', {@testCS3_fcn});
testCS4 = uibutton(fig, 'Text', 'Test CS4', 'FontSize',11, 'Position',[360 180 100 40], 'ButtonPushedFcn', {@testCS4_fcn});
testlaser = uibutton(fig, 'Text', 'Test Laser', 'FontSize',11, 'Position',[470 180 100 40], 'ButtonPushedFcn', {@testlaser_fcn});
testvacuum = uibutton(fig, 'Text', 'Test Vacuum', 'FontSize',11, 'Position',[580 180 100 40], 'ButtonPushedFcn', {@testvacuum_fcn});

solenoid1panel = uipanel(fig, 'Title', 'Solenoid 1','Units','normalized', 'Position', [0.02 0.11 0.12 0.08]);
manualsolenoid1 = uibutton(solenoid1panel, 'Text','Manual', 'FontSize', 11, 'Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid1_fcn});
primesolenoid1 = uibutton(solenoid1panel, 'state', 'Text','Prime', 'Value', false, 'FontSize',11, 'Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid1_fcn});

solenoid2panel = uipanel(fig, 'Title', 'Solenoid 2','Units','normalized','Position', [0.02 0.02 0.12 0.08]);
manualsolenoid2 = uibutton('Parent', solenoid2panel, 'Text','Manual', 'FontSize', 11, 'Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid2_fcn});
primesolenoid2 = uibutton(solenoid2panel, 'state','Text','prime', 'Value', false, 'FontSize',11, 'Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid2_fcn});

solenoid3panel = uipanel(fig, 'Title', 'Solenoid 3','Units','normalized','Position', [0.15 0.11 0.12 0.08]);
manualsolenoid3 = uibutton('Parent', solenoid3panel, 'Text','Manual', 'FontSize', 11, 'Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid3_fcn});
primesolenoid3 = uibutton(solenoid3panel, 'state','Text','prime', 'Value', false, 'FontSize',11, 'Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid3_fcn});

solenoid4panel = uipanel(fig, 'Title', 'Solenoid 4','Units','normalized','Position', [0.15 0.02 0.12 0.08]);
manualsolenoid4 = uibutton('Parent', solenoid4panel, 'Text','Manual', 'FontSize', 11, 'Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid4_fcn});
primesolenoid4 = uibutton(solenoid4panel, 'state','Text','prime', 'Value', false, 'FontSize',11, 'Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid4_fcn});

lickretractsolenoid1panel = uipanel(fig, 'Title', 'Lick retract solenoid 1','Units','normalized','Position', [0.28 0.11 0.12 0.08]);
manuallickretractsolenoid1 = uibutton('Parent', lickretractsolenoid1panel, 'Text','Manual', 'FontSize', 11, 'Position', [15 28 120 20],'ButtonPushedFcn', {@manuallickretractsolenoid1_fcn});
primelickretractsolenoid1 = uibutton(lickretractsolenoid1panel,'state', 'Text','prime', 'Value', false, 'FontSize',11,'Position',[15 5 120 20], 'ValueChangedFcn', {@primelickretractsolenoid1_fcn});

lickretractsolenoid2panel = uipanel(fig, 'Title', 'Lick retract solenoid 2','Units','normalized','Position', [0.28 0.02 0.12 0.08]);
manuallickretractsolenoid2 = uibutton('Parent', lickretractsolenoid2panel, 'Text','Manual', 'FontSize', 11, 'Position', [15 28 120 20],'ButtonPushedFcn', {@manuallickretractsolenoid2_fcn});
primelickretractsolenoid2 = uibutton(lickretractsolenoid2panel, 'state','Text','prime', 'Enable','on', 'Value', 0, 'FontSize',11, 'Position',[15 5 120 20], 'ValueChangedFcn', {@primelickretractsolenoid2_fcn});

vacuumpanel = uipanel(fig, 'Title', 'Vacuum','Units','normalized','Position', [0.41 0.11 0.12 0.08]);
manualvacuum = uibutton(vacuumpanel, 'Text', 'Manual', 'FontSize', 11, 'Position', [15 12 120 30]);


% Send, Start and Stop buttons
sendbutton = uibutton(fig,'Text','Send','Position',[760 220 100 40],'Enable','off','ButtonPushedFcn', {@pushSend,connectbutton,connectfield,disconnectbutton,refreshbutton});
startbutton = uibutton(fig,'Text','Start','Position',[870 220 100 40],'Enable','off','ButtonPushedFcn', {@pushStart,filenamefield});
stopbutton = uibutton(fig,'Text','Stop','Position',[980 220 100 40],'Enable','off','ButtonPushedFcn', {@pushStop});

set(stopbutton,'ButtonPushedFcn', {@pushStop,startbutton,filenamefield});
set(uploadbutton,'ButtonPushedFcn', {@pushUpload,availablePorts,connectbutton});
set(connectbutton,'ButtonPushedFcn', {@pushConnect,availablePorts,connectfield,disconnectbutton,refreshbutton,sendbutton});
set(disconnectbutton,'ButtonPushedFcn', {@pushDisconnect,connectbutton,connectfield,refreshbutton,availablePorts,sendbutton});

end


function pushUpload(source,eventdata,availablePorts, selectedmode)%,pushSolenoid3,primeSolenoid3on,primeSolenoid3off,testVacuum,testLaser,testSerialPort,testCue1,testCue2,testCue3,testCue4,testCue5)    port = get(availablePorts,'Value');        % find which is selected
    
    port = get(availablePorts,'Value');        % find which is selected
    basecmd = strcat('"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -C"C:\Program Files (x86)\Ardu ino\hardware\tools\avr/etc/avrdude.conf" -v -patmega2560 -cwiring -P',port,' -b115200 -D -Uflash:w:');

    if selectedmode == 1
        [status,cmdout] = dos(strcat(basecmd,'D:\uploads\Namlab_behavior_cues.ino.hex',':i'));
    elseif selectedmode == 2
        [status,cmdout] = dos(strcat(basecmd,'D:\uploads\Namlab_behavior_randomrewards.ino.hex',':i'));
    elseif selectedmode == 3
        [status,cmdout] = dos(strcat(basecmd,'D:\uploads\Namlab_behavior_lickforreward.ino.hex',':i'));
    elseif selectedmode == 4
        [status,cmdout] = dos(strcat(basecmd,'D:\uploads\Namlab_behavior_decisionmaking.ino.hex',':i'));
    elseif selectedmode == 5
        [status,cmdout] = dos(strcat(basecmd,'D:\uploads\Serial_port_testing.ino.hex',':i'));
    elseif selectedmode == 6
        [status,cmdout] = dos(strcat(basecmd,'D:\uploads\Namlab_behavior_ramptiming.ino.hex',':i'));    
    elseif selectedmode == 7
        [status,cmdout] = dos(strcat(basecmd,'D:\uploads\Namlab_behavior_delaydiscounting_automated.ino.hex',':i'));    
    end


    if contains(cmdout, 'avrdude done.') && status==0
        set(uploadButton, 'Text', 'Successfully uploaded');
        set(experimentmode, 'Enable', 'off');
        set(uploadButton, 'Enable','off');
        set(connectButton, 'Enable','on');
    else
        set(uploadButton, 'Text', 'Unable to upload');
        pause(5);
        set(uploadButton, 'Text', 'Upload');
    end
end

function pushConnect(source, eventdata, availablePorts,connectfield,disconnectbutton,refreshbutton,sendbutton)
    global s

    portList = get(availablePorts,'Items');    % get list from popup menu
    port = get(availablePorts,'Value');         % find which is selected

    s = serial(port,'BaudRate',57600,'Timeout',1);      % setup serial port with arduino, specify the terminator as a LF ('\n' in Arduino)
    fopen(s)                                            % open serial port with arduino
    
    set(connectfield,'Value',port);                     % write out port selected in menu
    set(availablePorts,'Enable','off');                 % Disable drop down menu of ports
    set(source,'Enable','off');
    set(connectbutton, 'Enable','off');
    set(disconnectbutton,'Enable','on');
    set(refreshbutton,'Enable','off');
    set(sendbutton, 'Enable', 'on');
        
    selectedmode = get(experimentmode, 'Value');
    if selectedmode == 1 || selectedmode == 4 || selectedmode ==6
        set(handles.sendButton,'Enable','on') 
        set(handles.csproperties, 'Enable', 'on');
        set(handles.checkboxtrialbytrial, 'Enable', 'on');
        set(handles.checkboxrandlaser, 'Enable', 'on');
        set(handles.lasertrialbytrial, 'Enable', 'on');
        set(handles.laserlatency, 'Enable', 'on');
        set(handles.laserduration, 'Enable', 'on');
        set(handles.laserpulseperiod, 'Enable', 'on');
        set(handles.laserpulseoffperiod, 'Enable', 'on');
        set(handles.CS1lasercheck, 'Enable', 'on');
        set(handles.CS2lasercheck, 'Enable', 'on');
        set(handles.CS3lasercheck, 'Enable', 'on');
        set(handles.CS4lasercheck, 'Enable', 'on');
        set(handles.Rewardlasercheck, 'Enable', 'on');
        set(handles.checkboxintervaldistribution, 'Enable', 'on');
        set(handles.maxdelaycuetovacuum, 'Enable', 'on');
        set(handles.meanITI, 'Enable', 'on');
        set(handles.maxITI, 'Enable', 'on');
        set(handles.minITI, 'Enable', 'on');
        set(handles.backgroundsolenoid, 'Enable', 'on');
        set(handles.T_bgd, 'Enable', 'on');
        set(handles.r_bgd, 'Enable', 'on');
        set(handles.mindelaybgdtocue, 'Enable', 'on'); 
        set(handles.mindelayfxdtobgd, 'Enable', 'on');    
    elseif selectedmode == 2
        set(handles.sendButton,'Enable','on') 
        set(handles.backgroundsolenoid, 'Enable', 'on');
        set(handles.T_bgd, 'Enable', 'on');
        set(handles.r_bgd, 'Enable', 'on');
        set(handles.mindelaybgdtocue, 'Enable', 'on'); 
        set(handles.mindelayfxdtobgd, 'Enable', 'on');
        set(handles.totPoisssolenoid, 'Enable', 'on');
    elseif selectedmode == 3 || selectedmode == 7
        set(handles.sendButton,'Enable','on') 
        set(handles.lickproperties, 'Enable', 'on');
    elseif selectedmode == 5
        set(handles.testserialport, 'Enable', 'on');
    end

end

function pushDisconnect(source, eventdata,connectbutton,connectfield,refreshbutton,availablePorts,sendbutton)
    global s
    fclose(s)
    instrreset                                          % "closes serial"
    set(source,'Enable','off');
    set(connectfield,'Value','Disconnected');
    set(connectbutton,'Enable','on');
    set(refreshbutton,'Enable','on');
    set(availablePorts,'Enable','on');
    set(sendbutton, 'Enable', 'off');

end

% --- Executes on button press in refreshButton.
function pushRefresh(source, eventdata, availablePorts, experimentmode)

    serialInfo = instrhwinfo('serial');                             % get info on connected serial ports
    port = serialInfo.AvailableSerialPorts;

    % get names of ports
    if ~isempty(port)
        set(availablePorts,'Items',port)                            % update list of ports available
    else
        set(availablePorts,'Items', ...
            'none found, please check connection and refresh')      % if none, indicate so
    end
end


%Send button callback
function pushSend(source,eventdata,connectbutton,connectfield,disconnectbutton,refreshbutton,startbutton,testbuttons, csproperties, lickproperties) 
    global s
    numtrials    = cell2mat(csproperties(1,2:end));
    CSfreq       = cell2mat(csproperties(2,2:end));
    CSsolenoid   = [str2double(split(csproperties(3,2),'+'))',...
                    str2double(split(csproperties(3,3),'+'))',...
                    str2double(split(csproperties(3,4),'+'))',...
                    str2double(split(csproperties(3,5),'+'))'];
    CSprob       = [str2double(split(csproperties(4,2),'+'))',...
                    str2double(split(csproperties(4,3),'+'))',...
                    str2double(split(csproperties(4,4),'+'))',...
                    str2double(split(csproperties(4,5),'+'))'];
    CSopentime   = [str2double(split(csproperties(5,2),'+'))',...
                    str2double(split(csproperties(5,3),'+'))',...
                    str2double(split(csproperties(5,4),'+'))',...
                    str2double(split(csproperties(5,5),'+'))'];
    CSdur        = cell2mat(csproperties(6,2:end));
    CS_t_fxd     = [str2double(split(csproperties(7,2),'+'))',...
                    str2double(split(csproperties(7,3),'+'))',...
                    str2double(split(csproperties(7,4),'+'))',...
                    str2double(split(csproperties(7,5),'+'))'];
    CSpulse      = cell2mat(csproperties(8,2:end));
    CSspeaker    = cell2mat(csproperties(9,2:end));
    CSlight      = cell2mat(csproperties(10,2:end));
    golickreq    = cell2mat(csproperties(11,2:end));
    golicktube   = cell2mat(csproperties(12,2:end));
    CSsignal     = cell2mat(csproperties(13, 2:end));
    CSrampmaxdelay = cell2mat(csproperties(14, 2:end));
    CSrampexp    = cell2mat(csproperties(15,2:end));
    CSincrease = cell2mat(csproperties(16, 2:end));
    delayforsecondcue = cell2mat(csproperties(17, 2:end));
    secondcuetype = cell2mat(csproperties(18, 2:end));
    secondcuefreq = cell2mat(csproperties(19, 2:end));
    secondcuespeaker = cell2mat(csproperties(20, 2:end));
    secondcuelight = cell2mat(csproperties(21,2:end));
    
    
    verify_inputs
    

    params = sprintf('%G+',cueprob,cuetype,cuefreq,cuesource,cuedur,cuepulse,ISI,laserpulse,...
              laserdelay,laserdur,vacdelay,vacdur,sesdur,timedses);
    params = params(1:end-1);
    
    
    %Send params to arduino
    fprintf(s,params);  % send info to arduino
    flushinput(s)
    
    %Make params uneditable
    set(uit,'Enable','off','ColumnEditable',false); %Make table uneditable
    set(source,'Enable','off');     %Disable Send button
    set(startbutton,'Enable','on'); %Enable start button
    %set(resetbutton,'Enable','on'); %Enable Reset button
    set(vacdelayfield,'Enable','off','Editable','off');
    set(vacdurfield,'Enable','off','Editable','off');
    set(sesdurbtn,'Enable','off');
    set(sesdurfield,'Enable','off','Editable','off');
    set(numrewbtn,'Enable','off');
    set(numrewfield,'Enable','off','Editable','off');
    
    set(connectbutton,'Enable','off');
    set(connectfield,'Enable','off');
%     set(disconnectbutton,'Enable','off');
    set(refreshbutton,'Enable','off');
    
    %Turn on testing buttons
    for btn = testbuttons
        set(btn,'Enable','on');
    end    
end  

function pushReset(source,eventdata,uploadbutton,sendbutton,startbutton) 
    %Make params editable
    set(uit,'Enable','on','ColumnEditable',true); %Make table editable
    set(sendbutton,'Enable','on'); %Enable Send button
    set(vacdelayfield,'Enable','on','Editable','on');
    set(vacdurfield,'Enable','on','Editable','on');
    set(sesdurbtn,'Enable','on');
    set(sesdurfield,'Enable','on','Editable','on');
    set(numrewbtn,'Enable','on');
    set(numrewfield,'Enable','on','Editable','on');
    
    
    set(startbutton,'Enable','off');    %Disable start button
    set(source,'Enable','off');         %Disable Reset button
end

function pushStart(source,eventdata,uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,filenamefield,counters,testbuttons,controlbuttons,starttimefield) 
    
    global s running actvAx saveDir 
    
    verify_inputs
    
    params = sprintf('%G+',cueprob,cuetype,cuefreq,cuesource,cuedur,cuepulse,ISI,laserpulse,...
              laserdelay,laserdur,vacdelay,vacdur,sesdur,timedses)
             
    params = params(1:end-1);

    %Reset counters to 0
    for counter = counters
        set(counter,'Value',0);
    end

    fname = get(filenamefield,'Value');
    
    % Run arduino code
    fprintf(s,'0');   % Signals to Arduino to start the experiment
    set(source,'Enable','off');             %Disable start button
    set(controlbuttons(1),'Enable','on');   %Enable stop button
    
    conditioning_prog_cueseq
    
    %Disable control buttons
    for btn = controlbuttons
        set(btn,'Enable','off');    %reset, serial port drop down, refresh, connect, upload, file name field, disconnect
    end
    
    
    
    %Turn off testing buttons
    for btn = testbuttons
        set(btn,'Enable','off');
    end
    
    
    flushinput(s);                                  % clear serial input buffer 
end

function pushStop(source,eventdata,startbutton,filenamefield) 
    global s running
    running = false;            % Stop running MATLAB code for monitoring arduino
    fprintf(s,'1');             % Send stop signal to arduino; 49 in the Arduino is the ASCII code for 1

    set(source,'Enable','off');
    %set(resetbutton,'Enable','on');
    set(filenamefield,'Enable','on','Editable','on');
    
    %Close Serial Port
    fclose(s)
    instrreset                                          % "closes serial"
end

% Test buttons for cues and laser 
function testCS1_fcn(source, eventdata)
    global s 
    fprintf(s,'2');              % Send CS2 signal to arduino; 51 in the Arduino is the ASCII code for 3
    flushinput(s)
end
function testCS2_fcn(source, eventdata)
    global s 
    fprintf(s,'3');              % Send CS2 signal to arduino; 51 in the Arduino is the ASCII code for 3
    flushinput(s)
end
function testCS3_fcn(source, eventdata)
    global s 
    fprintf(s,'4');              % Send CS2 signal to arduino; 51 in the Arduino is the ASCII code for 3
    flushinput(s)
end
function testCS4_fcn(source, eventdata)
    global s 
    fprintf(s,'5');              % Send CS2 signal to arduino; 51 in the Arduino is the ASCII code for 3
    flushinput(s)
end
function testlaser_fcn(source, eventdata)
    global s 
    fprintf(s,'8');              % Send CS2 signal to arduino; 51 in the Arduino is the ASCII code for 3
    flushinput(s)
end
function testvacuum_fcn(source, eventdata)
    global s
    fprintf(s, '9');
    flushinput(s);
end

% Test buttons for solenoids
function manualsolenoid1_fcn(source, eventdata)
    global s
    fprintf(s, 'A');
end
function primesolenoid1_fcn(source, eventdata)
    global s
    
    if source.Value == 1
        fprintf(s,'B');              % Send prime solenoid signal to arduino; 66 in the Arduino is the ASCII code for B
    else
        fprintf(s,'C');              % Send stop solenoid signal to arduino; 67 in the Arduino is the ASCII code for C
    end
end

function manualsolenoid2_fcn(source, eventdata)
    global s
    fprintf(s, 'D');
end
function primesolenoid2_fcn(source, eventdata)
    global s
    if source.Value == 1
        fprintf(s,'E');              % Send prime solenoid signal to arduino; 69 in the Arduino is the ASCII code for E
    else
        fprintf(s,'F');              % Send stop solenoid signal to arduino; 70 in the Arduino is the ASCII code for F
    end
end

function manualsolenoid3_fcn(source, eventdata)
    global s
    fprintf(s, 'G');
end
function primesolenoid3_fcn(source, eventdata)
    global s
    if source.Value == 1
        fprintf(s,'H');              % Send prime solenoid signal to arduino; 69 in the Arduino is the ASCII code for E
    else
        fprintf(s,'I');              % Send stop solenoid signal to arduino; 70 in the Arduino is the ASCII code for F
    end
end

function manualsolenoid4_fcn(source, eventdata)
    global s
    fprintf(s, 'J');
end
function primesolenoid4_fcn(source, eventdata)
    global s
    if source.Value == 1
        fprintf(s,'K');              % Send prime solenoid signal to arduino; 69 in the Arduino is the ASCII code for E
    else
        fprintf(s,'L');              % Send stop solenoid signal to arduino; 70 in the Arduino is the ASCII code for F
    end
end

function manuallickretractsolenoid1_fcn(source, eventdata)
    global s
    fprintf(s, 'M');
end
function primelickretractsolenoid1_fcn(source, eventdata)
    global s
    if source.Value == 1
        fprintf(s,'N');              % Send prime solenoid signal to arduino; 69 in the Arduino is the ASCII code for E
    else
        fprintf(s,'O');              % Send stop solenoid signal to arduino; 70 in the Arduino is the ASCII code for F
    end
end

function manuallickretractsolenoid2_fcn(source, eventdata)
    global s
    fprintf(s, 'P');
end
function primelickretractsolenoid2_fcn(source, eventdata)
    global s
    if source.Value == 1
        fprintf(s,'Q');              % Send prime solenoid signal to arduino; 69 in the Arduino is the ASCII code for E
    else
        fprintf(s,'R');              % Send stop solenoid signal to arduino; 70 in the Arduino is the ASCII code for F
    end
end
