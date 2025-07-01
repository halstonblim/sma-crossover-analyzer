import yfinance as yf

# 2) Download data for one symbol
df = yf.download("AAPL",
                 start="2020-01-01", 
                 end="2025-06-30", 
                 interval="1d")
# 3) Write to CSV
df.to_csv("AAPL_daily.csv")