import asyncio

async def my_function():
    # Your asynchronous code here
    print("Async function executed")

async def delayed_execution(delay_seconds):
    # Sleep for the specified number of seconds
    await asyncio.sleep(delay_seconds)
    await my_function()

async def main():
    # Set the delay in seconds (e.g., 3 seconds)
    delay_seconds = 3

    # Run the delayed_execution coroutine
    await delayed_execution(delay_seconds)

if __name__ == "__main__":
    # Create an event loop
    loop = asyncio.get_event_loop()

    # Run the main coroutine
    loop.run_until_complete(main())

    # Close the event loop
    loop.close()
