import pandas as pd
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import LabelEncoder

# Load dataset
df = pd.read_csv('dataset.csv')

# Convert categorical variables to numeric
le_gender = LabelEncoder()
le_dietary = LabelEncoder()
le_reminder = LabelEncoder()
le_risk = LabelEncoder()

df['Gender'] = le_gender.fit_transform(df['Gender'])
df['Dietary Intake'] = le_dietary.fit_transform(df['Dietary Intake'])
df['Reminder Frequency'] = le_reminder.fit_transform(df['Reminder Frequency'])
df['Risk Level'] = le_risk.fit_transform(df['Risk Level'])

# Features and target
X = df[['Age', 'Gender', 'Hemoglobin Level', 'Dietary Intake', 'Exercise Frequency', 'Reminder Frequency']]
y = df['Risk Level']

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Create linear regression model
model = LinearRegression()

# Train the model
model.fit(X_train, y_train)

# Make predictions
y_pred = model.predict(X_test)

# Evaluate the model
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)

print(f'Mean Squared Error: {mse:.2f}')
print(f'R-squared: {r2*100:.2f}')