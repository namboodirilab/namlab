%% Runs session 
% This function is called upon clicking test serial port button in GUI
% Requires variables param and s (serial object).

cla;     % Clear the axes before starting
%% Setup
% timeslot = readcell('timeslot.xlsx');
% expectedts = cell2mat(timeslot(:,2));
numts = 0;
expectedts = NaN(1001, 1);
tempts = NaN(1001,1);
percent_error = NaN(1001, 1);
timect = 0;

%% Set axis and load arduino
running = true;                                     % variable to control program
axes(actvAx)
set(actvAx,'YLim', [-10 10],...
    'XLim',[0 900]);

%% Start
while running
        read = [];
        if s.BytesAvailable > 0 % is data available to read? This avoids the timeout problem
            read = fscanf(s,'%u');% scan for data sent only when data is available
        end
        if isempty(read)
            drawnow
            continue
        end        
        code = read(1);                             % read identifier for data
        if code == 0                                % signifies "end of session"
            break
        end
        
        time = read(2);
        timect = timect + 1;
        
        % code = 25; Test serial port connection       
        if code == 25 && numts <= 1000  
            numts = numts + 1; 
            tempts(numts, 1) = time;
            expectedts(numts, 1) = 0+3*(numts-1);
            percent_error(numts, 1) = (numts - timect)*100/(numts);                       
        end
        % start figure for plots
        plot(expectedts, percent_error, '-k.', 'LineWidth', 1);
        xlabel('Time (ms)');
        ylabel('% Error of dropped signal');
        drawnow      
end

%% Save data 
portList = get(handles.availablePorts,'String');    % get list from popup menu
selected = get(handles.availablePorts,'Value');     % find which is selected
port     = portList{selected};                      % selected port

    assignin('base', 'expectedts', expectedts);
    format = 'yymmdd-HHMMSS';
    date = datestr(now,format);
    varname = strcat('tempts',port);
    assignin('base', varname, tempts);
    fname = 'Serialtest';
    file = [saveDir fname '_' port '_' date '.mat'];
    save(file, 'tempts')
