# Airport System

This project implements a flight management system that allows the definition and consultation of airports and flights, as well as making reservations and removing flights and reservations from the system. The passing of time is controlled by the application through a command that allows you to set the date current system. When the program starts, the current date is considered to be January 1st 2022. 

Note that the program is in portuguese.

## Learning Objective

The main objective of this project was to learn how to create and use data structures as well as properly manage memory, as there are no limits on the number of reservations or the size of the reservation codes. Valgrind was utilized to check for possible memory leaks.  \
We were only allowed to use the libraries `stdio.h`, `stdlib.h`, `ctype.h` and `string.h`.

## Commands

- **`q`**: Exits the program
- **`a`** _`<airportID> <country> <city>`_: Adds a new airport to the system.  \
Prints out `airport <airportID>` on success.
- **`l`** `[<airportID> <airportID> ...]`: Lists airports.  \
Prints out `<airportID> <city> <country> #flights` for every airport in alphabetic order of their ID's if there was no argument, or for only the airports provided as an argument.
- **`v`** _`[<flightCode> <departureAirportID> <arrivalAirportID> <departureDate> <departureHour> <duration> <capacity>]`_: Adds a new flight or lists all flights.  \
Prints out `<flightCode> <departureAirportID> <arrivalAirportID> <departureDate> <departureHour>` for every flight ordered by creation time, or nothing if a new flight was created.
- **`p`** _`<airportID>`_: Lists all flights that depart from an airport.  \
Prints out `<flightCode> <arrivalAirportID> <departureData> <departureHour>` for every flight, ordered by date and hour of departure, from oldest to newest.
- **`c`** _`<airportID>`_: Lists all flights that arrive at a certain airport.  \
Prints out `<flightCode> <departureAirportID> <arrivalDate> <arrivalHour>` for every flight, ordered by date and hour of arrival, from oldest to newest.
- **`t`** _`date`_: Advances the system's date.  \
Prints out `<date>`, where is the current date's new value.
- **`r`** _`<flightCode> <date> [<reservationCode> <nPassengers>]`_: Makes a new reservation or lists a flight's reservations.  \
Prints out `<reservationCode> <nPassenger>` for every reservation for the flight with the code `<flightCode>` for the date `<date>`, ordered alphabetically by reservation code.
- **`e`** _`<code>`_: Removes flights or reservations.  \
Deletes all flights or the reservation with the code `<code>`. If it's a flight code, it also deletes all reservations associated with said flight.

#### Specifications:

An airport's ID is composed of 3 capitalized letters, a _`country`_ is a non-empty string with up to 30 characters and a _`city`_ is a non-empty string with up to 50 characters. A flight code is composed of 2 capitalized letters followed by a number between 1 and 9999, each flight can have between 10 and 1000 passengers (inclusive), and a flight's duration can never exceed 12 hours. There can be a maximum of 40 airports and 30000 flights, but there is no limit on the number of reservations or the length of reservation codes. Dates are in the format DD-MM-YYYY, and hours are in the format HH:MM.