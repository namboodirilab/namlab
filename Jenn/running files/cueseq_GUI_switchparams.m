function cueseq_GUI

global s running actvAx saveDir

mainPath = 'C:\Users\namboodirilab\OneDrive - University of California, San Francisco\Behavioral_acquisition_and_analysis';
addpath(mainPath)
saveDir = [mainPath '\data\']          % where to save data


% Make figure
sz = get(0, 'ScreenSize'); % screen size
x = mean(sz([1, 3])); % center position
y = mean(sz([2, 4])); % center position
width = 1500;
height = 1000;

fig = uifigure('Position', [x - width/2, y - height/2, width, height]);


colnames = {'Cue 1', 'Cue 2', 'Cue 3', 'Cue 4', 'Cue 5', 'Reward'};
rownames = {'P(Cue 1 given Column)', 'P(Cue 2 given Column)', 'P(Cue 3 given Column)', 'P(Cue 4 given Column)',...
            'P(Cue 5 given Column)','P(Reward given Column)','Sound(1) or light(2)','Frequency (Hz) or 0 if light',...
            'Light or speaker number','Cue/solenoid durationm (ms)','Pulse cue? (ms on/ms off)','Inter-state interval (ms)','Laser pulse? (ms on/ms off)','Laser delay wrt cue onset', 'Laser dur'};
default = {  50, 10, 0, 0, 0, 0; %P(Cue 1 given Column)
          20, 0, 20, 0, 0, 100; %P(Cue 2 given Column)
          0, 90, 80, 0, 0, 0; %P(Cue 3 given Column)
          30, 0, 0, 0, 0, 0; %P(Cue 4 given Column)
          0, 0, 0, 100, 0, 0; %P(Cue 5 given Column)
          0, 0, 0, 0, 100, 0; %P(Reward given Column)
          1, 1, 1, 2, 2, 'N/A'; %Sound(1) or light(2)
          12000, 3000, 5000, 0, 0, 'N/A'; %Frequency (Hz) or 0 if light           
          1, 2, 2, 1, 1, 'N/A'; %Light or speaker number
          1000, 1000, 1000, 1000, 1000, 40; %Cue/solenoid durationm (ms)
          '0/0','0/0','200/200','0/0','200/200','N/A'; %Pulse cue? (ms on/ms off)
          500, 500, 500, 500, 500, 5000; %Inter-state interval
          '0/0','0/0','0/0','0/0','0/0','0/0'; %Laser pulse? (ms on/ms off)
          0,0,0,0,0,0; %Laser delay wrt cue on (ms)
          0,0,0,0,0,0; %Laser duration (ms)
          };

% Make data table
global uit 
uit = uitable(fig,'Data',default);
set(uit, 'columnname', colnames);
set(uit, 'rowname', rownames);
uit.Position(:) = [20 400 700 360];
uit.ColumnEditable = true;

s = uistyle('BackgroundColor','#FFE5CC');
addStyle(uit,s,'row',[1:6])


%Make other options/settings (i.e. pulse, laser, lick requirements)

%Make Session Length fields
bg = uibuttongroup(fig,'Title','Session length:','Position',[20 250 200 70]);
sesdurbtn = uiradiobutton(bg,'Text','Duration (min):','Position',[10 25 150 20]);
sesdurfield = uieditfield(bg,'numeric','Value',60, 'Position', [120 25 70 22]);
numrewbtn = uiradiobutton(bg,'Text','# of Rewards:','Position',[10 5 150 20]);
numrewfield = uieditfield(bg,'numeric','Value',100, 'Position', [120 5 70 22]);

%Make Vacuum fields
vacdelayfield = uieditfield(fig,'numeric','Value',0, 'Position', [180 370 70 22]);
vacdelaylbl = uilabel(fig, 'Text', 'Vacuum delay wrt solenoid: ', 'Position', [20 370 180 22]);
vacdurfield = uieditfield(fig,'numeric','Value',0, 'Position', [180 345 70 22]);
vacdurlbl = uilabel(fig, 'Text', 'Vacuum duration: ', 'Position', [20 345 180 22]);

% Find available serial ports
availablePorts = uidropdown(fig,'Position',[150 200 100 22]);    %Make serial port drop down
availablePortslbl = uilabel(fig,'Text','Select Serial Port:','Position',[20 200 130 22]);
serialInfo = instrhwinfo('serial');
port = serialInfo.AvailableSerialPorts;
if ~isempty(port)
    set(availablePorts,'Items',port)
end

%Refresh serial port button
refreshbutton = uibutton(fig, 'Position',[260 200 70 22], 'Text','Refresh','ButtonPushedFcn', {@pushRefresh,availablePorts});

%Connected to Field
connectfield = uieditfield(fig,'text','Editable','off','Position', [180 140 150 22]);
connectlbl = uilabel(fig, 'Text', 'Connected to:', 'Position', [100 140 80 22]);

%Connect Button - open selected serial port and send arduino file
connectbutton = uibutton(fig, 'Position',[20 140 70 22], 'Text','Connect','Enable','off','ButtonPushedFcn', {@pushConnect,availablePorts,connectfield});

% Disconnect button
disconnectbutton = uibutton(fig, 'Position',[350 140 70 22], 'Text','Disconnect','Enable','off','ButtonPushedFcn', {@pushDisconnect,connectbutton,connectfield,refreshbutton,availablePorts});

%Make reset button
%resetbutton = uibutton(fig,'Text','Reset','Position',[20 30 100 22],'Enable','off');

%Upload Button - send arduino file to arduino
uploadfield = uieditfield(fig,'text','Editable','off','Position', [100 170 150 22]);
uploadbutton = uibutton(fig, 'Position',[20 170 70 22], 'Text','Upload','ButtonPushedFcn', {@pushUpload,availablePorts,uploadfield,connectbutton});%pushSolenoid3,primeSolenoid3on,primeSolenoid3off,testVacuum,testLaser,testSerialPort,testCue1,testCue2,testCue3,testCue4,testCue5});

%Make save file field
filenamefield = uieditfield(fig,'text','Value','JD_behavior_M','Editable','on','Position', [100 100 150 22]);
filenamelbl = uilabel(fig, 'Text', 'Save file as:', 'Position', [20 100 70 22]);

%Make stop button
stopbutton = uibutton(fig,'Text','Stop','Position',[130 30 100 22],'Enable','off','ButtonPushedFcn', {@pushStop});
 
%Make start button
startbutton = uibutton(fig,'Text','Start','Position',[130 60 100 22],'Enable','off','ButtonPushedFcn', {@pushStart,uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,filenamefield});
starttimelbl = uilabel(fig, 'Text', 'Start Time:', 'Position', [240 60 70 22]);
starttimefield = uieditfield(fig,'text','Editable','off','Position', [320 60 50 22]);

%Make send button
sendbutton = uibutton(fig,'Text','Send','Position',[20 60 100 22],'Enable','on','ButtonPushedFcn',...
    {@pushSend, uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,...
    connectbutton,connectfield,disconnectbutton,refreshbutton,startbutton});

%Set callbacks
%set(resetbutton,'ButtonPushedFcn',{@pushReset,uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,uploadbutton,uploadfield,sendbutton,startbutton});
set(stopbutton,'ButtonPushedFcn', {@pushStop,startbutton,filenamefield});
set(uploadbutton,'ButtonPushedFcn', {@pushUpload,availablePorts,uploadfield,connectbutton});
set(connectbutton,'ButtonPushedFcn', {@pushConnect,availablePorts,connectfield,disconnectbutton,refreshbutton,sendbutton});
set(disconnectbutton,'ButtonPushedFcn', {@pushDisconnect,connectbutton,connectfield,refreshbutton,availablePorts,sendbutton});


%Make Plot
ax = uiaxes(fig, 'Position', [750 160 700 800]);
actvAx = ax;    % set as global so conditiong_prog can plot

%Make plot legend

%Make behavior counters
cue1count = uieditfield(fig,'numeric','Value',0,'BackgroundColor','r','Position', [900 130 70 22],'Enable','on','Editable','off');
cue1countlbl = uilabel(fig, 'Text', 'Cue 1 count: ','FontColor','r', 'Position', [770 130 150 22]);

cue2count = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','g','Position', [900 110 70 22],'Enable','on','Editable','off');
cue2countlbl = uilabel(fig, 'Text', 'Cue 2 count: ', 'FontColor','g', 'Position', [770 110 150 22]);

cue3count = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','b','Position', [900 90 70 22],'Enable','on','Editable','off');
cue3countlbl = uilabel(fig, 'Text', 'Cue 3 count: ', 'FontColor','b', 'Position', [770 90 150 22]);

cue4count = uieditfield(fig,'numeric','Value',0,'BackgroundColor','c', 'Position', [900 70 70 22],'Enable','on','Editable','off');
cue4countlbl = uilabel(fig, 'Text', 'Cue 4 count: ', 'FontColor','c', 'Position', [770 70 150 22]);

cue5count = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','m','Position', [900 50 70 22],'Enable','on','Editable','off');
cue5countlbl = uilabel(fig, 'Text', 'Cue 5 count: ', 'FontColor','m', 'Position', [770 50 150 22]);

rewcount = uieditfield(fig,'numeric','Value',0, 'Position', [900 30 70 22],'Enable','on','Editable','off');
rewcountlbl = uilabel(fig, 'Text', 'Reward count: ','FontColor','k',  'Position', [770 30 150 22]);

lickcount = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','#7E2F8E','Position', [1150 10 70 22],'Enable','on','Editable','off');
lickcountlbl = uilabel(fig, 'Text', 'Total Lick count: ',	'FontColor','#7E2F8E', 'Position', [990 10 150 22]);

state1lickcount = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','r','Position', [1150 130 70 22],'Enable','on','Editable','off');
state1lickcountlbl = uilabel(fig, 'Text', 'State 1 lick count: ',	'FontColor','r', 'Position', [990 130 150 22]);

state2lickcount = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','g','Position', [1150 110 70 22],'Enable','on','Editable','off');
state2lickcountlbl = uilabel(fig, 'Text', 'State 2 lick count: ',	'FontColor','g', 'Position', [990 110 150 22]);

state3lickcount = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','b','Position', [1150 90 70 22],'Enable','on','Editable','off');
state3lickcountlbl = uilabel(fig, 'Text', 'State 3 lick count: ',	'FontColor','b', 'Position', [990 90 150 22]);

state4lickcount = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','c','Position', [1150 70 70 22],'Enable','on','Editable','off');
state4lickcountlbl = uilabel(fig, 'Text', 'State 4 lick count: ',	'FontColor','c', 'Position', [990 70 150 22]);

state5lickcount = uieditfield(fig,'numeric','Value',0, 'BackgroundColor','m','Position', [1150 50 70 22],'Enable','on','Editable','off');
state5lickcountlbl = uilabel(fig, 'Text', 'State 5 lick count: ',	'FontColor','m', 'Position', [990 50 150 22]);

rewstatelickcount = uieditfield(fig,'numeric','Value',0, 'Position', [1150 30 70 22],'Enable','on','Editable','off');
rewstatelickcountlbl = uilabel(fig, 'Text', 'Reward state lick count: ','FontColor','k', 'Position', [990 30 150 22]);

counters = [cue1count,cue2count,cue3count,cue4count,cue5count,rewcount,lickcount,...
            state1lickcount,state2lickcount,state3lickcount,state4lickcount,state5lickcount,rewstatelickcount];

%add counters to callbacks input arguments


%Make testing buttons
testsolenoid3button= uibutton(fig,'Text','Test Solenoid 3','Position',[580 130 100 22],'Enable','off','ButtonPushedFcn', {@pushSolenoid3});
primesolenoid3onbutton = uibutton(fig,'Text','On','Position',[680 100 30 22],'Enable','off','ButtonPushedFcn', {@primeSolenoid3on});
primesolenoid3offbutton = uibutton(fig,'Text','Off','Position',[710 100 30 22],'Enable','off','ButtonPushedFcn', {@primeSolenoid3off});
primesolenoid3lbl = uilabel(fig, 'Text', 'Prime solenoid 3:', 'Position', [580 100 100 22]);

testvacuumbutton = uibutton(fig,'Text','Test Vacuum','Position',[580 70 100 22],'Enable','off','ButtonPushedFcn', {@testVacuum});
testlaserbutton = uibutton(fig,'Text','Test Laser','Position',[580 40 100 22],'Enable','off','ButtonPushedFcn', {@testLaser});
testserialbutton = uibutton(fig,'Text','Test Serial Port','Position',[580 10 100 22],'Enable','off','ButtonPushedFcn', {@testSerialPort,availablePorts});

testcue1button = uibutton(fig,'Text','Test Cue 1','Position',[470 130 100 22],'Enable','off','ButtonPushedFcn', {@testCue1});
testcue2button = uibutton(fig,'Text','Test Cue 2','Position',[470 100 100 22],'Enable','off','ButtonPushedFcn', {@testCue2});
testcue3button = uibutton(fig,'Text','Test Cue 3','Position',[470 70 100 22],'Enable','off','ButtonPushedFcn', {@testCue3});
testcue4button = uibutton(fig,'Text','Test Cue 4','Position',[470 40 100 22],'Enable','off','ButtonPushedFcn', {@testCue4});
testcue5button = uibutton(fig,'Text','Test Cue 5','Position',[470 10 100 22],'Enable','off','ButtonPushedFcn', {@testCue5});

testbuttons = [testsolenoid3button,primesolenoid3onbutton,primesolenoid3offbutton,testvacuumbutton,testlaserbutton,testserialbutton,...
    testcue1button,testcue2button,testcue3button,testcue4button,testcue5button];


%add test buttons to callbacks input arguments
set(startbutton,'ButtonPushedFcn', {@pushStart,uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,filenamefield,counters,testbuttons,...
    [stopbutton,availablePorts,refreshbutton,connectbutton,uploadbutton,filenamefield,disconnectbutton],starttimefield});
set(sendbutton,'ButtonPushedFcn', {@pushSend,uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,connectbutton,connectfield,disconnectbutton,refreshbutton,startbutton,testbuttons});

end

%Upload cuesequence arduino file & enable testing buttons if successful
function pushUpload(source,eventdata,availablePorts,uploadfield,connectbutton)%,pushSolenoid3,primeSolenoid3on,primeSolenoid3off,testVacuum,testLaser,testSerialPort,testCue1,testCue2,testCue3,testCue4,testCue5)

    port = get(availablePorts,'Value');        % find which is selected
    
    basecmd = strcat('"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -patmega2560 -cwiring -P',port,' -b115200 -D -Uflash:w:');
    
    [status,cmdout] = dos(strcat(basecmd,'C:\Users\namboodirilab\Desktop\Behavioral_acquisition_and_analysis\uploads\Namlab_cuesequence.ino.hex',':i'));

    set(source, 'Enable','off');
    
    if contains(cmdout, 'avrdude done.') && status==0
        set(uploadfield, 'Value', 'Successfully uploaded'); 
        set(connectbutton, 'Enable', 'on');
    else
        set(uploadfield, 'Value', 'Unable to upload');
        pause(5);
        set(source, 'Enable', 'on');
    end
        
end

%Open serial port
function pushConnect(source, eventdata, availablePorts,connectfield,disconnectbutton,refreshbutton,sendbutton)
    global s

    portList = get(availablePorts,'Items');    % get list from popup menu
    port = get(availablePorts,'Value');         % find which is selected

    s = serial(port,'BaudRate',57600,'Timeout',1);      % setup serial port with arduino, specify the terminator as a LF ('\n' in Arduino)
    fopen(s)                                            % open serial port with arduino
    
    set(connectfield,'Value',port);                     % write out port selected in menu
    set(availablePorts,'Enable','off');                 % Disable drop down menu of ports
    set(source,'Enable','off');
    set(disconnectbutton,'Enable','on');
    set(refreshbutton,'Enable','off');
    set(sendbutton, 'Enable', 'on');
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
function pushRefresh(source, eventdata, availablePorts)

    serialInfo = instrhwinfo('serial');                             % get info on connected serial ports
    port = serialInfo.AvailableSerialPorts;
    port
    % get names of ports
    if ~isempty(port)
        set(availablePorts,'Items',port)                            % update list of ports available
    else
        set(availablePorts,'Items', ...
            'none found, please check connection and refresh')      % if none, indicate so
    end
end

%Send button callback
function pushSend(source,eventdata,uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,connectbutton,connectfield,disconnectbutton,refreshbutton,startbutton,testbuttons) 
    global s
    
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

function pushReset(source,eventdata,uit,vacdelayfield,vacdurfield,sesdurbtn,numrewbtn,sesdurfield,numrewfield,uploadbutton,uploadfield,sendbutton,startbutton) 
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
% test    
%     param = regexprep(params, '+', ' ');
%     param = str2num(param)
%     
%     params = struct();
%     paramnames = string({'cueprob_by_column';'cuetype';'cuefreq';'cuesource';'cuedur';'cuepulse';'ISI';'laserpulse';...
%               'laserdelay';'laserdur';'vacdelay';'vacdur';'sesdur'});
% 
%     params.(paramnames(1)) = param(1:36);                        % cue prob (36) - saved in column order of input table (i.e. 1st 6 #s are P(state x|cue1))
%     params.(paramnames(2)) = param(37:41);                       % cue type (5)
%     params.(paramnames(3)) = param(42:46);                       % cue freq (5)
%     params.(paramnames(4)) = param(47:51);                       % cue source (5)
%     params.(paramnames(5)) = param(52:57);                       % cue dur (6)
%     params.(paramnames(6)) = param(58:67);                       % cue pulse on/off (10)
%     params.(paramnames(7)) = param(68:73);                       % ISI (6)
%     params.(paramnames(8)) = param(74:85);                       % laser pulse on/off (12)
%     params.(paramnames(9)) = param(86:91);                       % laser delay wrt cue (6)
%     params.(paramnames(10)) = param(92:97);                      % laser dur (6)
%     params.(paramnames(11)) = param(98);                         % vac delay wrt solenoid (1)
%     params.(paramnames(12)) = param(99);                         % vac dur (1)
%     params.(paramnames(13)) = param(100);                        % ses dur (1)
%     params.(paramnames(14)) = param(101);                        % timedses (1) - 1 if session duration is timed, 0 if max rewards
     
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

% --- turn on solenoid 3 for fixed duration
function pushSolenoid3(source, eventdata)
    global s 
    fprintf(s, 'G');        % Send solenoid signal to arduino; 71 in the Arduino is the ASCII code for G
end

function primeSolenoid3on(source, eventdata)
    global s 
    fprintf(s, 'H');            % Send prime solenoid signal to arduino; 72 in the Arduino is the ASCII code for H
end

function primeSolenoid3off(source, eventdata)
    global s
    fprintf(s, 'I');            % Send prime solenoid signal to arduino; 73 in the Arduino is the ASCII code for I
end

function testLaser(source, eventdata)
    global s
    fprintf(s,'8');              % Send Laser signal to arduino; 56 in the Arduino is the ASCII code for 8
    flushinput(s)
end

function testSerialPort(source, eventdata, availablePorts)
    global s running actvAx saveDir

    mainPath = 'C:\Users\namboodirilab\Desktop\Behavioral_acquisition_and_analysis';	
    addpath(mainPath)	
    saveDir = [mainPath '\serialtest\'];          % save serial testing data here

    fprintf(s,'T');                     % Send test serial port signal to arduino; 84 in the Arduino is the ASCII code for T -- currently doesn't do anything in Arduino
    %set(source,'Enable','off');
    test_serial_port                    % Start figure for serial port connection signal
    %set(source,'Enable','on');
end

function testVacuum(source,eventdata)
    global s
    fprintf(s,'V');              % Send vacuum signal to arduino; 86 in the Arduino is the ASCII code for V
end

function testCue1(source, eventdata)
    global s
    fprintf(s,'2');              % Send CS1 signal to arduino; 50 in the Arduino is the ASCII code for 2
    flushinput(s)
end

function testCue2(source, eventdata)
    global s
    fprintf(s,'3');              % Send CS2 signal to arduino; 51 in the Arduino is the ASCII code for 3
    flushinput(s)
end

function testCue3(source, eventdata)
    global s
    fprintf(s,'4');              % Send CS3 signal to arduino; 52 in the Arduino is the ASCII code for 4
    flushinput(s)
end

function testCue4(source, eventdata)
    global s
    fprintf(s,'5');              % Send CS4 signal to arduino; 53 in the Arduino is the ASCII code for 5
    flushinput(s)
end

function testCue5(source, eventdata)
    global s
    fprintf(s,'6');              % Send CS5 signal to arduino; 54 in the Arduino is the ASCII code for 6
    flushinput(s)
end
