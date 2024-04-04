
% Read the soil moisture channel data from the past two weeks.
% Send an email and tell the user to add water if the value
% is in the lowest 10 %.

% Store the channel ID for the moisture sensor channel.
channelID = 2491729;

% Placeholder for your Read API Key
readAPIKey = '8LLCN118JX17ZNTK'; 

% Provide the ThingSpeak alerts API key.  All alerts API keys start with TAK.
alertApiKey = 'TAKC648dtVhulJdYit3';

% Set the address for the HTTTP call
alertUrl="https://api.thingspeak.com/alerts/send";

% webwrite uses weboptions to add required headers.  Alerts needs a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey ]);

% Set the email subject.
alertSubject = sprintf("Irrigation action needed");

% Read the recent data.
moistureData = thingSpeakRead(channelID, 'NumPoints', 1, 'Fields', 1, 'ReadKey', readAPIKey);

% Check to make sure the data was read correctly from the channel.
if isempty(moistureData)
    alertBody = 'No data read';
else
    lastValue = moistureData(end);
    dangerLevelHigh = 620;
    dangerLevelLow = 220;

    % Set the outgoing message
    if lastValue > dangerLevelHigh
        alertBody = sprintf('Moisture is in level of %0.2f. You should stop irrigation', lastValue);
    elseif (lastValue<dangerLevelLow)
        alertBody = sprintf('Moisture is in level of %0.2f. You should start irrigation', lastValue);
    end
end

 % Catch errors so the MATLAB code does not disable a TimeControl if it fails
try
    webwrite(alertUrl , "body", alertBody, "subject", alertSubject, options);
catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
