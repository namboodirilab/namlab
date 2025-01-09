function behavior_GUI

global s running actvAx saveDir textfield fig

mainPath = 'D:\OneDrive - UCSF\Behavioral_acquisition_and_analysis';
addpath(mainPath)
saveDir = [mainPath '\data\'];          % where to save data


% Make figure
sz = get(0, 'ScreenSize'); % screen size
x = mean(sz([1, 3])); % center position
y = mean(sz([2, 4])); % center position
width = 750;
height = 470;

fig = uifigure('Position', [x - width/2, y - height/2, width, height]);
fig.Units = 'normalized';
fig.Name = 'Behavior_GUI';

% Find available serial ports
availablePortslbl = uilabel(fig);
availablePortslbl.Text='Select Serial Port:';
% availablePortslbl.Units = 'normalized';
availablePortslbl.Position = [10 850/2 100 24];
availablePorts = uidropdown(fig);
availablePorts.Position = [110 850/2 170 24];    % Make serial port drop down
port = serialportlist("available");
if ~isempty(port)
    set(availablePorts,'Items',port)
end

%Connected to Field
connectlbl = uilabel(fig, 'Text', 'Connected to:', 'Position', [300 850/2 80 22]);
connectfield = uieditfield(fig,'text','Editable','off','Position', [390 850/2 150 22]);
refreshbutton = uibutton(fig, 'Position',[width*0.52 height*0.6 width*0.15 height*0.1], 'Text','Refresh','FontSize',11, 'ButtonPushedFcn', {@pushRefresh,availablePorts});
connectbutton = uibutton(fig, 'Position',[width*0.01 height*0.6 width*0.15 height*0.1], 'Text','Connect','FontSize',11, 'Enable','off','ButtonPushedFcn', {@pushConnect,connectfield,availablePorts});
uploadbutton = uibutton(fig, 'Position',[width*0.18 height*0.6 width*0.15 height*0.1], 'Text','Upload','FontSize',11, 'ButtonPushedFcn', {@pushUpload,availablePorts,connectbutton});
disconnectbutton = uibutton(fig, 'Position',[width*0.35 height*0.6 width*0.15 height*0.1], 'Text','Disconnect','FontSize',11, 'Enable','off','ButtonPushedFcn', {@pushDisconnect,connectbutton,connectfield,uploadbutton,refreshbutton,availablePorts});

solenoid1panel = uipanel(fig, 'Title', 'Solenoid 1','Units','normalized', 'Position', [0.01 0.2 0.2 0.26]);
opentimesolenoid1text = uilabel(solenoid1panel,'Text', 'open time', 'FontSize', 11, 'Position', [15 74 60 20]);
opentimesolenoid1 = uieditfield('numeric','Parent',solenoid1panel,'Editable','on','Position', [70 74 40 20]);
manualsolenoid1 = uibutton(solenoid1panel, 'Text','Manual', 'FontSize', 11, 'Enable','off','Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid1_fcn});
primesolenoid1 = uibutton(solenoid1panel, 'state', 'Text','Prime', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid1_fcn});
testsolenoid1 = uibutton(solenoid1panel, 'state', 'Text','Test', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 51 120 20], 'ValueChangedFcn', {@testsolenoid1_fcn});

solenoid2panel = uipanel(fig, 'Title', 'Solenoid 2','Units','normalized','Position', [0.22 0.2 0.2 0.26]);
opentimesolenoid1text = uilabel(solenoid2panel,'Text', 'open time', 'FontSize', 11, 'Position', [15 74 60 20]);
opentimesolenoid2 = uieditfield('numeric','Parent',solenoid2panel,'Editable','on','Position', [70 74 40 20]);
manualsolenoid2 = uibutton('Parent', solenoid2panel, 'Text','Manual', 'FontSize', 11, 'Enable','off','Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid2_fcn});
primesolenoid2 = uibutton(solenoid2panel, 'state','Text','Prime', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid2_fcn});
testsolenoid2 = uibutton(solenoid2panel, 'state', 'Text','Test', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 51 120 20], 'ValueChangedFcn', {@testsolenoid2_fcn});


solenoid3panel = uipanel(fig, 'Title', 'Solenoid 3','Units','normalized','Position', [0.44 0.2 0.2 0.26]);
opentimesolenoid1text = uilabel(solenoid3panel,'Text', 'open time', 'FontSize', 11, 'Position', [15 74 60 20]);
opentimesolenoid3 = uieditfield('numeric','Parent',solenoid3panel,'Editable','on','Position', [70 74 40 20]);
manualsolenoid3 = uibutton('Parent', solenoid3panel, 'Text','Manual', 'FontSize', 11, 'Enable','off','Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid3_fcn});
primesolenoid3 = uibutton(solenoid3panel, 'state','Text','Prime', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid3_fcn});
testsolenoid3 = uibutton(solenoid3panel, 'state', 'Text','Test', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 51 120 20], 'ValueChangedFcn', {@testsolenoid3_fcn});

solenoid4panel = uipanel(fig, 'Title', 'Solenoid 4','Units','normalized','Position', [0.66 0.2 0.2 0.26]);
opentimesolenoid1text = uilabel(solenoid4panel,'Text', 'open time', 'FontSize', 11, 'Position', [15 74 60 20]);
opentimesolenoid4 = uieditfield('numeric','Parent',solenoid4panel,'Editable','on','Position', [70 74 40 20]);
manualsolenoid4 = uibutton('Parent', solenoid4panel, 'Text','Manual', 'FontSize', 11, 'Enable','off','Position', [15 28 120 20],'ButtonPushedFcn', {@manualsolenoid4_fcn});
primesolenoid4 = uibutton(solenoid4panel, 'state','Text','Prime', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 5 120 20], 'ValueChangedFcn', {@primesolenoid4_fcn});
testsolenoid4 = uibutton(solenoid4panel, 'state', 'Text','Test', 'Value', false, 'FontSize',11, 'Enable','off','Position',[15 51 120 20], 'ValueChangedFcn', {@testsolenoid4_fcn});

sendbutton = uibutton(fig,'Text','Send','FontSize', 12,'Position',[900 350 120 50],'Enable','off','ButtonPushedFcn', {@pushSend,...
    disconnectbutton,refreshbutton,testbuttons, manualbutton});

set(uploadbutton,'ButtonPushedFcn', {@pushUpload,availablePorts,uploadbutton,connectbutton});
set(connectbutton,'ButtonPushedFcn', {@pushConnect,connectbutton,availablePorts,connectfield,disconnectbutton,refreshbutton,sendbutton,cstable,licktable,Optopanel,ITIpanel,bgdrpanel,experimentmode});
set(disconnectbutton,'ButtonPushedFcn', {@pushDisconnect,connectbutton,connectfield,uploadbutton,refreshbutton,availablePorts,experimentmode,sendbutton,testbuttons,manualbuttons});
set(sendbutton, 'ButtonPushedFcn', {@pushSend,disconnectbutton,refreshbutton, testbuttons,manualbuttons});
set(fig, 'CloseRequestFcn', @closeFigureCallback);
end

function pushUpload(source, eventdata, availablePorts,uploadbutton,connectbutton)%,pushSolenoid3,primeSolenoid3on,primeSolenoid3off,testVacuum,testLaser,testSerialPort,testCue1,testCue2,testCue3,testCue4,testCue5)   
    port = get(availablePorts,'Value');        % find which is selected
    basecmd = strcat('"C:\Users\namboodirilab\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/bin/avrdude" "-CC:\Users\namboodirilab\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf" -v -V -patmega2560 -cwiring -P',port,' -b115200 -D "-Uflash:w:');    
    [status,cmdout] = dos(strcat(basecmd,'C:\Users\namboodirilab\Desktop\uploads\Namlab_behavior_cues.ino.hex',':i'));

    if contains(cmdout, 'avrdude done.') && status==0
        set(uploadbutton, 'Text', 'Successfully uploaded');
        set(experimentmode, 'Enable', 'off');
        set(uploadbutton, 'Enable','off');
        set(connectbutton, 'Enable','on');
    else
        set(uploadbutton, 'Text', 'Unable to upload');
        pause(5);
        set(uploadbutton, 'Text', 'Upload');
    end
end

function pushConnect(source,eventdata,connectbutton,availablePorts,connectfield,disconnectbutton,refreshbutton,sendbutton) 
    global s

%     portList = get(availablePorts,'Items');    % get list from popup menu
    port = get(availablePorts,'Value');         % find which is selected
    s = serial(port,'BaudRate',57600,'Timeout',1);      % setup serial port with arduino, specify the terminator as a LF ('\n' in Arduino)
    fopen(s);                                           % open serial port with arduino
    set(connectbutton,'Text','Wait 5s');
    pause(5);
    set(connectbutton,'Text','Connected');
    
%     set(connectfield,'String','Link');
    set(connectfield,'Value',port);                     % write out port selected in menu
    set(availablePorts,'Enable','off');                 % Disable drop down menu of ports
    set(source,'Enable','off');
    set(connectbutton, 'Enable','off');
    set(disconnectbutton,'Enable','on');
    set(refreshbutton,'Enable','off');
    set(sendbutton, 'Enable', 'on');
end

function pushDisconnect(source, eventdata, connectbutton,connectfield,uploadbutton,refreshbutton,availablePorts,sendbutton,testbuttons,manualbuttons)
    global s
%     fclose(s);
    delete(s);

    set(source,'Enable','off');
    set(connectfield,'Value','Disconnected');
    set(connectbutton,'Enable','on');
    set(refreshbutton,'Enable','on');
    set(availablePorts,'Enable','on');
    set(uploadbutton, 'Enable', 'on');
    set(uploadbutton, 'Text',  'Upload');
    set(experimentmode, 'Enable', 'on');
    set(sendbutton, 'Enable', 'off');
%     set(startbutton, 'Enable', 'off');
    set(testbuttons,'Enable','off');
    set(manualbuttons, 'Enable','off');
end

% --- Executes on button press in refreshButton.
function pushRefresh(source, eventdata, availablePorts)

%     serialInfo = serialportlist(ports);                             % get info on connected serial ports
    port = serialportlist("available");

    % get names of ports
    if ~isempty(port)
        set(availablePorts,'Items',port)                            % update list of ports available
    else
        set(availablePorts,'Items', ...
            'none found, please check connection and refresh')      % if none, indicate so
    end
end


%Send button callback
function pushSend(source,eventdata,disconnectbutton,refreshbutton, testbuttons, manualbuttons)
    global s
    CSopentime = [get(opentimesolenoid1, 'Value'),...
                    get(opentimesolenoid2, 'Value')',...
                    get(opentimesolenoid3, 'Value'),...
                    get(opentimesolenoid4, 'Value')];
    % Validate inputs
    params = sprintf('%G+',CSopentime);
    params = params(1:end-1);

    %Send params to arduino
    fprintf(s,params);  % send info to arduino
    flushinput(s)
    
    %Make params uneditable
    set(source,'Enable','off');     %Disable Send button
    set(disconnectbutton,'Enable','on');
    set(refreshbutton,'Enable','off');

    %Turn on testing buttons
    for btn = testbuttons
        set(btn,'Enable','on');
    end  
    for btn = manualbuttons
        set(btn, 'Enable','on');
    end
end


function testsolenoid1_fcn(source, eventdata)
    global s
    fprintf(s, 'M'); %77
end
function testsolenoid2_fcn(source, eventdata)
    global s
    fprintf(s, 'N'); %78
end
function testsolenoid3_fcn(source, eventdata)
    global s
    fprintf(s, 'O'); %79
end
function testsolenoid4_fcn(source, eventdata)
    global s
    fprintf(s, 'P'); %80
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


function closeFigureCallback(fig,event)
    % Close all serial ports
    delete(fig);
    ports = instrfindall;
    if ~isempty(ports)
        fclose(ports);
        delete(ports);
    end
    % Close the GUI or perform any other necessary actions
end
