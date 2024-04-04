% Enter your MATLAB Code below
% Read the soil moisture channel data from the past two weeks.
% Send an email and tell the user to add water if the value
% is in the lowest 10 %.

% Store the channel ID for the moisture sensor channel.
channelID = *******;

% Placeholder for your Read API Key
readAPIKey = '****************'; % Replace YOUR_READ_API_KEY_HERE with your actual Read API Key

% Provide the ThingSpeak alerts API key.  All alerts API keys start with TAK.
alertApiKey = '*******************';

% Set the address for the HTTTP call
alertUrl="https://api.thingspeak.com/alerts/send";

% webwrite uses weboptions to add required headers.  Alerts needs a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey ]);

% Set the email subject.
alertSubject = sprintf(" Temperature Alert: Action Required on Farm");

% Read the recent data.
Temperature = thingSpeakRead(channelID,'NumDays',1,'Field',2, 'ReadKey', readAPIKey);

% Check to make sure the data was read correctly from the channel.
if isempty(Temperature)
    alertBody = 'No data read from sensor';      
else
    % setting the 15 degrees threshold value.
    minValue = 15;
    maxValue = 26;

    % Get the most recent point in the array of moisture data.
    lastValue = Temperature(end); 

    % Set the outgoing message
    if (lastValue<=minValue)
        alertBody = sprintf('Your attention is required. The current temperature on your farm has dropped below 15°C, posing a risk to your crops and livestock. To prevent potential damage, please promptly close any open spaces allowing cold air in.Thank you for your immediate action.', lastValue);
    elseif (lastValue> 26)
        alertBody = sprintf('Your farms temperature has exceeded 26°C, posing potential risks to your crops and livestock. Please open all enclosed spaces to prevent heat buildup.', lastValue);
    end
end
 
 % Catch errors so the MATLAB code does not disable a TimeControl if it fails
try
    webwrite(alertUrl , "body", alertBody, "subject", alertSubject, options);
catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
