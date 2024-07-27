from flask import Flask, render_template, request, redirect, url_for
import sqlite3
from datetime import datetime

app = Flask(__name__)

def get_db_connection():
    conn = sqlite3.connect('blood_booster.db')
    conn.row_factory = sqlite3.Row
    return conn

@app.route('/')
def index():
    conn = get_db_connection()
    users = conn.execute('SELECT * FROM users').fetchall()
    conn.close()
    return render_template('index.html', users=users)

@app.route('/user/<int:user_id>')
def user_detail(user_id):
    conn = get_db_connection()
    user = conn.execute('SELECT * FROM users WHERE id = ?', (user_id,)).fetchone()
    logs = conn.execute('SELECT * FROM medication_log WHERE user_id = ? ORDER BY date DESC', (user_id,)).fetchall()
    conn.close()

    total_consumption = sum(log['quantity'] for log in logs)
    target_met = total_consumption >= user['target_daily']

    return render_template('user_detail.html', user=user, logs=logs, total_consumption=total_consumption, target_met=target_met)

@app.route('/add_user', methods=('GET', 'POST'))
def add_user():
    if request.method == 'POST':
        name = request.form['name']
        target_daily = request.form['target_daily']

        conn = get_db_connection()
        conn.execute('INSERT INTO users (name, target_daily) VALUES (?, ?)', (name, target_daily))
        conn.commit()
        conn.close()

        return redirect(url_for('index'))

    return render_template('add_user.html')

@app.route('/add_log/<int:user_id>', methods=('GET', 'POST'))
def add_log(user_id):
    if request.method == 'POST':
        date = request.form['date']
        quantity = request.form['quantity']

        conn = get_db_connection()
        conn.execute('INSERT INTO medication_log (user_id, date, quantity) VALUES (?, ?, ?)', (user_id, date, quantity))
        conn.commit()
        conn.close()

        return redirect(url_for('user_detail', user_id=user_id))

    return render_template('add_log.html', user_id=user_id)

@app.route('/edit_user/<int:user_id>', methods=['GET', 'POST'])
def edit_user(user_id):
    conn = get_db_connection()
    
    if request.method == 'POST':
        name = request.form['name']
        target_daily = request.form['target_daily']
        
        conn.execute('UPDATE users SET name = ?, target_daily = ? WHERE id = ?',
                     (name, target_daily, user_id))
        conn.commit()
        conn.close()
        return redirect(url_for('index'))
    
    user = conn.execute('SELECT * FROM users WHERE id = ?', (user_id,)).fetchone()
    conn.close()
    return render_template('edit_user.html', user=user)

@app.route('/delete_user/<int:user_id>', methods=['POST'])
def delete_user(user_id):
    conn = get_db_connection()
    conn.execute('DELETE FROM users WHERE id = ?', (user_id,))
    conn.execute('DELETE FROM medication_log WHERE user_id = ?', (user_id,))
    conn.commit()
    conn.close()
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(debug=True)
