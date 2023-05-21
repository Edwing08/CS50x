import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Keep in mind the user's id for future database requests
    id_user = session["user_id"]

    # Obtain information about stocks owned by the user
    stocks = db.execute(
        "SELECT symbol, name, shares, price FROM stock WHERE id IN (SELECT transactions_id FROM purchase WHERE users_id = ?)", id_user)

    # Obtain the user's current cash balance and calculate the total price
    cash = db.execute("SELECT cash FROM users WHERE id = ?", id_user)
    cash = cash[0]["cash"]

    totalPrice_stock = 0

    # Calcule the stock's total price based on the number of shares
    for stock in stocks:
        stock["total_price"] = stock["price"]*stock["shares"]
        totalPrice_stock += stock["price"]*stock["shares"]

    total = totalPrice_stock + cash

    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Process information inserted in the form via POST method
    if request.method == "POST":

        # Obtain the symbol and lookup the data related to it
        symbol = request.form.get("symbol")
        data = lookup(symbol)

        # If the symbol does not exist or if the input is blank, return an apology
        if (data == None):
            return apology("symbol does not exist")

        # Obtain the number of shares to be bought
        shares = request.form.get("shares")

        # Ensure the number of shares are positive and integer
        if not shares.isdigit():
            return apology("shares must be a positive and integer number", 400)
        else:
            shares = int(shares)
            if shares <= 0:
                return apology("shares must be a positive and integer number", 400)

        # Obtain the stock’s current price and name
        price = data["price"]
        name = data["name"]

        # Keep in mind the user's id for future database requests
        id_user = session["user_id"]

        # Obtain the user's current cash balance and calculate the total price
        cash = db.execute("SELECT cash FROM users WHERE id = ?", id_user)
        cash = cash[0]["cash"]
        total_price = shares*price

        # Apologize if the user cannot afford the purchase
        if (total_price > cash):
            return apology("not enough balance to execute the purchase")
        # purchase request
        else:
            # Deduct the cost of the purchase from the user's balance
            new_cash = cash - total_price

            # Obtain the current time in order to record the purchase in the database
            time_now = datetime.datetime.now()
            date = time_now.strftime("%d-%m-%y")
            time = time_now.strftime("%H:%M")

            # Update the user's balance
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, id_user)

            # Record the purchase in the database / table called transactions, which is related to the transactions' history
            id_transaction = db.execute(
                "INSERT INTO transactions (symbol, quantity, price, date, time, name, action) VALUES (?,?,?,?,?,?,?)", symbol, shares, price, date, time, name, "Buy")
            db.execute(
                "INSERT INTO purchase (users_id, transactions_id) VALUES (?,?)", id_user, id_transaction)

            # Record the purchase in the database / table called stock, which is related to the stocks owned by the user
            symbol_bought = db.execute(
                "SELECT symbol, id, shares FROM stock WHERE id IN (SELECT stock_id FROM owned WHERE users_id = ?) AND symbol = ?", id_user, symbol)

            # If it is the first time that the user buys the stock just purchased
            if not symbol_bought:
                id_stock = db.execute("INSERT INTO stock (symbol, name, shares, price) VALUES (?,?,?,?)",
                                      symbol, name, shares, price)
                db.execute("INSERT INTO owned (users_id, stock_id) VALUES (?,?)", id_user, id_stock)
            # If it is not the first time that the user buys the stock just purchased
            else:
                new_shares = symbol_bought[0]["shares"] + shares
                db.execute("UPDATE stock SET shares = ? WHERE id = ?", new_shares, symbol_bought[0]["id"])

            # Notify the user of the purchase
            return redirect("/")

    # Display the form if the get method is used
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Keep in mind the user's id for future database requests
    id_user = session["user_id"]

    # Obtain data related to every transaction made by the user
    transactions = db.execute(
        "SELECT * FROM transactions WHERE id IN (SELECT transactions_id FROM purchase WHERE users_id = ?)", id_user)

    for transaction in transactions:
        transaction["total_price"] = transaction["price"]*transaction["quantity"]

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):

            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # Obtain the symbol for quotation
        symbol = request.form.get("symbol")
        data = lookup(symbol)

        # Return an apology case the quoted symbol does not exist
        if(data == None):
            return apology("symbol does not exist", 400)

        return render_template("quoted.html", data=data)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Register if the post method is used
    if request.method == "POST":

        # Obtain the values from the form
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check for errors
        # Check for any space in blank
        if not username:
            print("true")
            return apology("muss provide username", 400)
        elif not password:
            return apology("muss provide password", 400)
        elif not confirmation:
            return apology("muss provide confirmation password", 400)

        # Check if the confirmation password match
        if not (password == confirmation):
            return apology("passwords do not match", 400)

        # Check if the username is already taken
        usernames_taken = db.execute("SELECT username FROM users")
        for users in usernames_taken:
            if username in users.values():
                return apology("username already in use", 400)

        # Hash the password
        password_hashed = generate_password_hash(password)

        # Insert new user to the database
        # Store the hash of the password instead of the plain text
        id = db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, password_hashed)

        # Keep the user logged in
        session["user_id"] = id

        # Redirect the user
        return redirect("/")

    # Display the form if the get method is used
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Process information inserted in the form via POST method
    if request.method == "POST":

        # Obtain the symbol and lookup the data related to it
        symbol = request.form.get("symbol")
        data = lookup(symbol)

        # Keep in mind the user's id for future database requests
        id_user = session["user_id"]

        # If the symbol does not exist or if the input is blank, return an apology
        if (data == None):
            return apology("symbol does not exist")

        # Obtain data about the stock to be sold
        symbol_existence = db.execute(
            "SELECT symbol,shares, price, id FROM stock WHERE id IN (SELECT transactions_id FROM purchase WHERE users_id = ?) AND symbol = ?", id_user, symbol)

        # Obtain the number of shares to be sold
        shares = request.form.get("shares", type=int)

        # Ensure the number of shares are positive and integer
        if (shares <= 0 and not isinstance(shares, int)):
            return apology("shares must be a positive and integer number")

        # Apologize if the user do not have enough shares to be sold
        if not (symbol_existence[0]["shares"] >= shares):
            return apology("not enough shares from this stock")
        # sale request
        else:
            # Obtain the stock’s current price and name
            price = data["price"]
            name = data["name"]

            # Obtain the user's current cash balance and total price
            cash = db.execute("SELECT cash FROM users WHERE id = ?", id_user)
            cash = cash[0]["cash"]
            total_price = shares*price

            # Deduct the cost of the purchase from the user's balance
            new_cash = cash + total_price

            # Obtain the current time in order to record the purchase in the database
            time_now = datetime.datetime.now()
            date = time_now.strftime("%d-%m-%y")
            time = time_now.strftime("%H:%M")

            # Update the user's balance
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, id_user)

            # Record the purchase in the database / table called transactions, which is related to the transactions' history
            id_transaction = db.execute(
                "INSERT INTO transactions (symbol, quantity, price, date, time, name, action) VALUES (?,?,?,?,?,?,?)", symbol, shares, price, date, time, name, "Sell")
            db.execute("INSERT INTO purchase (users_id, transactions_id) VALUES (?,?)", id_user, id_transaction)

            # Record the purchase in the database / table called stock, which is related to the stocks owned by the user
            new_shares = symbol_existence[0]["shares"] - shares

            # Delete the ownership of the stock sold if there are not shares from that stock left
            if new_shares == 0:
                db.execute("DELETE FROM owned WHERE users_id = ? AND stock_id = ?", id_user, symbol_existence[0]["id"])
                db.execute("DELETE FROM stock WHERE id = ?", symbol_existence[0]["id"])
            else:
                db.execute("UPDATE stock SET shares = ? WHERE id = ?", new_shares, symbol_existence[0]["id"])

            # Notify the user of the purchase
            return redirect("/")

    # Display the form if the get method is used
    else:
        id_user = session["user_id"]
        symbols = db.execute(
            "SELECT symbol FROM stock WHERE id IN (SELECT transactions_id FROM purchase WHERE users_id = ?)", id_user)

        return render_template("sell.html", symbols=symbols)
