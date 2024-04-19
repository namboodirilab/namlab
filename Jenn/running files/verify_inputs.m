    % This is a function for cueseq_GUI.m in @pushSend and @pushStart
    % Gets user data and validates inputs
    
    data = get(uit, 'Data');
    cueprob = cell2mat(data(1:6,:));
    cueprob = reshape(cueprob,1,[]);
    cuetype = cell2mat(data(7,1:5)); 
    cuefreq = cell2mat(data(8,1:5));
    cuesource = cell2mat(data(9,1:5));
    cuedur = cell2mat(data(10,:));
    cuepulse = [str2double(split(data(11,1),'/'))',str2double(split(data(11,2),'/'))',str2double(split(data(11,3),'/'))',...
                str2double(split(data(11,4),'/'))',str2double(split(data(11,5),'/'))'];    %cell2mat(data(12,1:5));
   
    ISI = cell2mat(data(12,:));
    laserpulse = [str2double(split(data(13,1),'/'))',str2double(split(data(13,2),'/'))',str2double(split(data(13,3),'/'))',...
                    str2double(split(data(13,4),'/'))',str2double(split(data(13,5),'/'))',str2double(split(data(13,6),'/'))'];       %cell2mat(data(13,:));
    laserdelay = cell2mat(data(14,:));
    laserdur = cell2mat(data(15,:));
    
    vacdelay = [get(vacdelayfield,'Value')];
    vacdur = [get(vacdurfield,'Value')];
    sesdurbtn_status = get(sesdurbtn,'Value');
    if sesdurbtn_status 
        sesdur = [get(sesdurfield,'Value')];
        timedses = 1;
    else
        sesdur = [get(numrewfield,'Value')];
        timedses = 0;
    end
    
    inputs = [cueprob,cuetype,cuefreq,cuesource,cuedur,cuepulse,ISI,laserpulse,...
              laserdelay,laserdur,vacdelay,vacdur,sesdur,timedses];
    
    %Verify inputs
    negIn  = inputs < 0;
    intIn  = inputs - fix(inputs);
    if any([negIn intIn])
        errordlg('Invalid inputs')
        error('Invalid inputs')
    end
    
    for i = 1:5
        if cuepulse((i*2)-1) > cuedur(i)
            errordlg('Cue pulse duration must be less than or equal to cue duration')
            error('Cue pulse duration must be less than or equal to cue duration')
        end
        
        if laserpulse((i*2)-1) > laserdur(i)
            errordlg('Laser pulse duration must be less than or equal to laser duration')
            error('Laser pulse duration must be less than or equal to laser duration')
        end
    end
    
  
    for i = 1:6
        if laserdelay(i) + laserdur(i) > cuedur(i) + ISI(i)
            errordlg('Laser duration must fall within cue presentation/ISI')
            error('Laser duration must fall within cue presentation/ISI')
        end   
    end