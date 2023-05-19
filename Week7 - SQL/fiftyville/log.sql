-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Crime description
SELECT description
    FROM crime_scene_reports
        WHERE month = 7 AND day = 28
        AND street = "Humphrey Street";

-- Interviews conducted on the same date as the crime
SELECT transcript
    FROM interviews
        WHERE month = 7 AND day = 28;

-- Car's Owners leaving the bakery parking lot around ten minutes after the crime
SELECT name, hour, minute
    FROM bakery_security_logs
        JOIN people
        ON bakery_security_logs.license_plate = people.license_plate
            WHERE month = 7 AND day = 28
            AND hour = 10 AND minute BETWEEN 15 AND 30;

-- Name of single person that withdrew money from the ATM located in "Leggett Street" the day of the crime
SELECT name, amount
    FROM bank_accounts
        JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number

        JOIN people
        ON bank_accounts.person_id = people.id

        WHERE month = 7 AND day = 28
        AND atm_location = "Leggett Street"
        AND transaction_type = "withdraw";

-- Every phonecall from the caller that took place the same day of the crime and last less than 60 seconds.
SELECT caller, name, duration
    FROM people
        JOIN phone_calls
        ON phone_calls.caller = people.phone_number
            WHERE month = 7 AND day = 28
            AND duration < 60;

-- Every phonecall from the receiver that took place the same day of the crime and last less than 60 seconds.
SELECT receiver, name, duration
    FROM people
        JOIN phone_calls
        ON phone_calls.receiver = people.phone_number
            WHERE month = 7 AND day = 28
            AND duration < 60;

-- The first flight out Fiftyville the next day of the crime
SELECT full_name, city, hour, minute
    FROM flights
        JOIN airports
        ON airports.id = flights.origin_airport_id
            WHERE month = 7 AND day = 29
            ORDER BY hour LIMIT 1;

-- The destination of the first flight from Fiftyville the next day of the crime
SELECT full_name, city, hour, minute
    FROM flights
        JOIN airports
        ON airports.id = flights.destination_airport_id
            WHERE month = 7 AND day = 29
            ORDER BY hour LIMIT 1;

-- Every person that boarded the first flight out Fiftyville the next day of the crime
SELECT name, city, hour, minute, seat
    FROM flights
        JOIN airports
        ON airports.id = flights.origin_airport_id

        JOIN passengers
        ON passengers.flight_id = flights.id

        JOIN people
        ON passengers.passport_number = people.passport_number
            WHERE month = 7 AND day = 29
            AND hour = 8 AND minute = 20;


