def lagrange_interpolation(x_values, y_values, x):
    result = 0
    for i in range(len(x_values)):
        term = y_values[i]
        for j in range(len(x_values)):
            if i != j:
                term *= (x - x_values[j]) / (x_values[i] - x_values[j])
        result += term
    return result

def test_interpolation(x_values, y_values, test_x_values):
    for test_x in test_x_values:
        result = lagrange_interpolation(x_values, y_values, test_x)
        percentage = (result / test_x) * 100
        print(f"P({test_x}) = {result}")
        print(f"Percentage from {test_x}: {percentage:.2f}%")
        print()

# Given data points
x_values = [1, 1000, 10000]
y_values = [0.2, 1, 5]

# Test the Lagrange interpolation polynomial for various x values
test_x_values = [1, 6, 10, 50, 100, 500, 1000, 5000, 10000]
test_interpolation(x_values, y_values, test_x_values)

# Good set
# x_values = [1, 1000, 10000]
# y_values = [0.2, 1, 5]





