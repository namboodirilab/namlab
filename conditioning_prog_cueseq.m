%% Runs session
% This function is called upon clicking the Start button in the CUESEQ GUI
% Requires variables param and s (serial object).

% cla;                                    % Clear the axes before starting

% fig = uifigure;
% fig.Position(3:4) = [1500 1000];
% 
% actvAx = uiaxes(fig, 'Position', [750 400 700 400]);

%% Parameters

% licksinit = ceil(sum(numtrials)*(truncITI+max(CS_t_fxd))*10/1E3);  % number of licks to initialize = number of trials*max time per trial in s*10Hz (of licking)
% cuesinit = sum(numtrials);                               % number of cues to initialize
logInit = 10^6;                                      % Log of all serial input events from the Arduino
% bgdsolenoidsinit = ceil(sum(numtrials)*truncITI*3/T_bgd);      % number of background solenoids to initialize = total time spent in ITI*rate of rewards*3. It won't be more than 3 times the expected rate
% 
% xWindow = [-(truncITI+1000) maxdelaycuetovacuum];  % Defines x-axis limits for the plot.
% fractionPSTHdisplay = 0.15;             % What fraction of the display is the PSTH?
% yOffset = ceil(fractionPSTHdisplay*sum(numtrials)/(1-fractionPSTHdisplay));% amount by which the y-axis extends beyond trials so as to plot the PSTH of licks
% binSize = 1000;                         % in ms
% xbins = xWindow(1):binSize:xWindow(2);  % Bins in x-axis for PSTH
% 
% ticks = -(truncITI+1000):10000:maxdelaycuetovacuum;% tick marks for x-axis of raster plot. moves through by 2s
% labels = ticks'/1000;                     % convert tick labels to seconds
% labelsStr = cellstr(num2str(labels));     % convert to cell of strings
% 
durationtrialpartitionnocues = 20E3;      % When experimentmode=2 or 3, how long should a single row for raster plot be? 20 sec
%% Prep work

% initialize arrays for licks and cues
lickct = 0;% Counter for total licks (solenoid 3)
statelickct = zeros(1,6);% Counters for licks during each state
vacuum = 0;% Counter for vacuums
cue1 = 0;% Counter for cue 1's
cue2 = 0;% Counter for cue 2's
cue3 = 0;% Counter for cue 3's
cue4 = 0;% Counter for cue 4's
cue5 = 0;% Counter for cue 5's
rew = 0; % Counter for reward state (solenoid 3)
eventlog = zeros(logInit,3);% empty event log for all events 
l = 0;% Counter for logged events
state = 1; %current state - keep track to record licks/state

% setup plot

plot(actvAx,[0 0;0 0],[0 0;-1 -1],'w');hold(actvAx,'on')
xlabel(actvAx,'time (s)');
ylabel(actvAx,' ');
xlim(actvAx,[-1000 durationtrialpartitionnocues+1000]);
set(actvAx,'ytick',[],...
           'xtick',0:2000:durationtrialpartitionnocues,...
            'XTickLabel',num2str((0:2000:durationtrialpartitionnocues)'/1000));


drawnow

%% Load to arduino

startT = clock;                                     % find time of start
startTStr = sprintf('%d:%d:%02.0f', ...
                    startT(4),startT(5),startT(6)); % format time
set(starttimefield,'Value',startTStr)                   % display time
drawnow

wID = 'MATLAB:serial:fscanf:unsuccessfulRead';      % warning id for serial read timeout
warning('off',wID)                                  % suppress warning

running = true;                                     % variable to control program
%%
try
   
%% Collect data from arduino
    while running
        read = [];
        
        if s.BytesAvailable > 0 % is data available to read? This avoids the timeout problem
            read = fscanf(s,'%f'); % scan for data sent only when data is available
        end
        
        if isempty(read)
            drawnow
            continue
        end

        l = l + 1;
        eventlog(l,:) = read;                      % maps three things from read (code/time/nosolenoidflag)
        
        time = read(2);                             % record timestamp
        nosolenoidflag = read(3);                     % if =1, no solenoid was actually given. Indicates solenoid omission
        code = read(1);                             % read identifier for data
        
        if code == 0                                % signifies "end of session"
            break
        end
        
         % Inputs from Arduino along with their "code" (defined below)
        %   1 = Lick1 onset
        %   2 = Lick1 offset
        %   3 = Lick2 onset
        %   4 = Lick2 offset                  
        %   5 = Lick3 onset
        %   6 = Lick3 offset
        %   7 = Background solenoid
        %   8 = Fixed solenoid 1
        %   9 = Fixed solenoid 2                
        %   10 = Fixed solenoid 3
        %   11 = Fixed solenoid 4 *****ask mingkang about this
        %   12 = Lick retract solenoid 1
        %   13 = Lick retract solenoid 2
        %**   14 = Vacuum   
        %   15 = CS1
        %   16 = CS2
        %   17 = CS3                    % leave room for possible cues 
        %   18 = CS4
        %   19 = CS5
        %   21 = Light 1
        %   22 = Light 2
        %   23 = light 3
        %   25 = both CSsound1 and CSlight1
        %   26 = both CSsound2 and CSlight2
        %   27 = both CSsound3 and CSlight3
        %   30 = frame
        %   31 = laser
        %   32 = solenoid off time
        %   33 = reward indicator 1, for CS1 or lick 1
        %   34 = reward indicator 2, for CS2 or lick 2
        
        %         counters = [cue1count,cue2count,cue3count,cue4count,cue5count,rewcount,lickcount,...
        %             state1lickcount,state2lickcount,state3lickcount,state4lickcount,state5lickcount,rewstatelickcount];
   
        if code == 5                                % Lick3 onset; PURPLE
            lickct = lickct + 1;
            set(counters(7),'Value',lickct);
            statelickct(state) = statelickct(state) + 1;
            set(counters(state+7),'Value', statelickct(state));
            trial = floor(time/durationtrialpartitionnocues);
            temptrialdur = trial*durationtrialpartitionnocues;   % it seems like this is just time...?             
            plot(actvAx,[time-temptrialdur;time-temptrialdur],[-trial;-trial-1],'Color','#7E2F8E','LineWidth',1);hold(actvAx,'on')
        
        elseif code == 10                            % Fixed solenoid 3; BLACK
            if nosolenoidflag == 0
               rew = rew + 1;
               set(counters(6),'Value',rew);
               state = 6;
               trial = floor(time/durationtrialpartitionnocues);
               temptrialdur = trial*durationtrialpartitionnocues;
               plot(actvAx,[time-temptrialdur;time-temptrialdur],[-trial;-trial-1],'Color','k','LineWidth',2);hold(actvAx,'on')
            end
            
%         elseif code == 14                            %Should I count/plot Vacuum?
        
        elseif code == 15                            % Cue1 onset; RED
            cue1 = cue1 + 1;
            set(counters(1),'Value',cue1);
            state = 1;
            trial = floor(time/durationtrialpartitionnocues);
            temptrialdur = trial*durationtrialpartitionnocues;
            plot(actvAx,[time-temptrialdur;time-temptrialdur],[-trial;-trial-1],'r','LineWidth',2);hold(actvAx,'on')
  
        elseif code == 16                            % Cue 2 onset; GREEN
            cue2 = cue2 + 1;         
            set(counters(2),'Value',cue2);  
            state = 2;
            trial = floor(time/durationtrialpartitionnocues);
            temptrialdur = trial*durationtrialpartitionnocues;
            plot(actvAx,[time-temptrialdur;time-temptrialdur],[-trial;-trial-1],'g','LineWidth',2);hold(actvAx,'on')
            
        elseif code == 17                            % Cue 3 onset; BLUE
            cue3 = cue3 + 1;         
            set(counters(3),'Value',cue3);
            state = 3;
            trial = floor(time/durationtrialpartitionnocues);
            temptrialdur = trial*durationtrialpartitionnocues;
            plot(actvAx,[time-temptrialdur;time-temptrialdur],[-trial;-trial-1],'b','LineWidth',2);hold(actvAx,'on')

        elseif code == 18                            % Cue 4 onset; CYAN
            cue4 = cue4 + 1;         
            set(counters(4),'Value',cue4);
            state = 4;
            trial = floor(time/durationtrialpartitionnocues);
            temptrialdur = trial*durationtrialpartitionnocues;
            plot(actvAx,[time-temptrialdur;time-temptrialdur],[-trial;-trial-1],'c','LineWidth',2);hold(actvAx,'on')

        elseif code == 19                            % Cue 5 onset; PINK
            cue5 = cue5 + 1;         
            set(counters(5),'Value',cue5);
            state = 5;
            trial = floor(time/durationtrialpartitionnocues);
            temptrialdur = trial*durationtrialpartitionnocues;
            plot(actvAx,[time-temptrialdur;time-temptrialdur],[-trial;-trial-1],'m','LineWidth',2);hold(actvAx,'on')          
        end
    end
    
    if l < logInit
        eventlog = eventlog(1:l,:);   % smaller eventlog
    end
        

%% Save data

    format = 'yymmdd-HHMMSS';
    date = datestr(now,format);
    expmt = 'cuesequence_';
    
    param = regexprep(params, '+', ' ');
    param = str2num(param);
    
    params = struct();
    paramnames = string({'cueprob_by_column';'cuetype';'cuefreq';'cuesource';'cuedur';'cuepulse';'ISI';'laserpulse';...
              'laserdelay';'laserdur';'vacdelay';'vacdur';'sesdur';'timedses'});

    params.(paramnames(1)) = param(1:36);                        % cue prob (36) - saved in column order of input table (i.e. 1st 6 #s are P(state x|cue1))
    params.(paramnames(2)) = param(37:41);                       % cue type (5)
    params.(paramnames(3)) = param(42:46);                       % cue freq (5)
    params.(paramnames(4)) = param(47:51);                       % cue source (5)
    params.(paramnames(5)) = param(52:57);                       % cue dur (6)
    params.(paramnames(6)) = param(58:67);                       % cue pulse on/off (10)
    params.(paramnames(7)) = param(68:73);                       % ISI (6)
    params.(paramnames(8)) = param(74:85);                       % laser pulse on/off (12)
    params.(paramnames(9)) = param(86:91);                       % laser delay wrt cue on(6)
    params.(paramnames(10)) = param(92:97);                      % laser dur (6)
    params.(paramnames(11)) = param(98);                         % vac delay wrt solenoid (1)
    params.(paramnames(12)) = param(99);                         % vac dur (1)
    params.(paramnames(13)) = param(100);                        % ses dur (1)
    params.(paramnames(14)) = param(101);                        % timedses (1) - 1 if session duration is timed, 0 if max rewards
    
    assignin('base','eventlog',eventlog);
%     file = [saveDir fname '_' num2str(r_bgd) '_' num2str(T_bgd) '_'  str probstr laserstr bgdsolenoidstr extinctionstr date '.mat'];
    file = [saveDir fname '_' expmt date '.mat'];
    save(file, 'eventlog', 'params')


catch exception
    if l < logInit
        eventlog = eventlog(1:l,:);
    end
    
    fprintf(s,'1');                                  % send stop signal to arduino; 49 in Arduino is the ASCII code for 1
    disp('Error running program.')
    format = 'yymmdd-HHMMSS';
    date = datestr(now,format);
    
    expmt = 'cuesequence_';
    
    assignin('base','eventlog',eventlog);  
%     file = [saveDir fname '_' num2str(r_bgd) '_' num2str(T_bgd) '_'  str probstr laserstr bgdsolenoidstr extinctionstr date '.mat'];
    file = [saveDir '_error_' fname '_' expmt date '.mat'];
       
    param = regexprep(params, '+', ' ');
    param = str2num(param);
    
    params = struct();

    paramnames = string({'cueprob_by_column';'cuetype';'cuefreq';'cuesource';'cuedur';'cuepulse';'ISI';'laserpulse';...
              'laserdelay';'laserdur';'vacdelay';'vacdur';'sesdur';'timedses'});

    params.(paramnames(1)) = param(1:36);                        % cue prob (36) - saved in column order of input table (i.e. 1st 6 #s are P(state x|cue1))
    params.(paramnames(2)) = param(37:41);                       % cue type (5)
    params.(paramnames(3)) = param(42:46);                       % cue freq (5)
    params.(paramnames(4)) = param(47:51);                       % cue source (5)
    params.(paramnames(5)) = param(52:57);                       % cue dur (6)
    params.(paramnames(6)) = param(58:67);                       % cue pulse on/off (10)
    params.(paramnames(7)) = param(68:73);                       % ISI (6)
    params.(paramnames(8)) = param(74:85);                       % laser pulse on/off (12)
    params.(paramnames(9)) = param(86:91);                       % laser delay wrt cue on (6)
    params.(paramnames(10)) = param(92:97);                      % laser dur (6)
    params.(paramnames(11)) = param(98);                         % vac delay wrt solenoid (1)
    params.(paramnames(12)) = param(99);                         % vac dur (1)
    params.(paramnames(13)) = param(100);                        % ses dur (1)
    params.(paramnames(14)) = param(101);                        % timedses (1) - 1 if session duration is timed, 0 if max rewards
    
    save(file, 'eventlog', 'params','exception')
end



% keep track of the last state code to know which state lick was in