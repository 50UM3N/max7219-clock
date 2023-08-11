import { IconPlugConnectedX } from "@tabler/icons-react";
import { useEffect, useState } from "preact/hooks";

enum MessageType {
    MODE = "0",
    SET_DATE_TIME = "1",
    SET_DATE = "2",
    SET_DATE_FORMAT = "3",
    SET_ANIMATION_SPEED = "4",
    SET_STAY_TIME = "5",
    SET_TEXT = "6",
}

function App() {
    const [ip, setIp] = useState(window.location.host);
    const [ipInput, setIpInput] = useState("");
    const [ipError, setIpError] = useState<null | string>(null);
    const [openApp, setOpenApp] = useState(false);
    const checkIpAddress = (ip: string) => {
        const ipAddressRegex = /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(?::\d+)?$/;
        if (!ipAddressRegex.test(ip)) {
            setIpError("Invalid IP address");
            return false;
        } else {
            setIpError(null);
        }
        return true;
    };
    const handleConnectToIp = () => {
        if (!checkIpAddress(ipInput)) return;
        setIp("");
        setIp(ipInput);
        setOpenApp(true);
    };

    return (
        <>
            <header className="container max-w-lg p-3 flex justify-between border-b border-mine-shaft-700 mb-4">
                <div className="px-3 py-1 rounded-lg bg-mine-shaft-100 text-mine-shaft-700 dark:text-mine-shaft-200 dark:bg-mine-shaft-600">Matrix Clock</div>
                {openApp && (
                    <button
                        onClick={() => {
                            setOpenApp(false);
                        }}
                        className="p-[6px] bg-mine-shaft-800 rounded-lg active:bg-mine-shaft-700 text-white"
                    >
                        <IconPlugConnectedX size="1.125rem" />
                    </button>
                )}
            </header>

            {!openApp ? (
                <div className="container max-w-lg px-3">
                    <div className="rounded-xl border border-mine-shaft-600 p-4 bg-mine-shaft-800">
                        <h2 className="text-white text-xl mb-1">Welcome in Matrix Clock</h2>

                        <p className="text-mine-shaft-400 mb-3">Connect with</p>
                        <div className="flex flex-col gap-3">
                            <input
                                type="text"
                                value={ipInput}
                                onChange={(e) => {
                                    checkIpAddress(e.currentTarget.value);
                                    setIpInput(e.currentTarget.value);
                                }}
                                placeholder="eg. 192.168.0.102"
                                className={`active:outline-none px-3 py-2 rounded-lg focus:outline-none border-2 border-m-green-500 focus:border-m-green-400 text-white bg-transparent ${ipError ? "border-red-500" : ""}`}
                            />

                            <div className="flex gap-3">
                                <button className="py-2 flex-1 px-3 bg-m-green-600 active:bg-m-green-500 rounded-lg border-2 border-m-green-600 active:border-m-green-500 text-white" onClick={handleConnectToIp}>
                                    Connect To Ip
                                </button>
                                <button
                                    className="py-2 px-3 border-2 flex-1 border-m-green-600 active:border-m-green-500 rounded-lg text-white"
                                    onClick={() => {
                                        setIpInput("");
                                        setOpenApp(true);
                                    }}
                                >
                                    Connect Default Ip
                                </button>
                            </div>
                        </div>
                    </div>
                </div>
            ) : (
                <Display ip={ip} />
            )}
        </>
    );
}

export default App;

const Display = ({ ip }: { ip: string }) => {
    const [ws, setWs] = useState<WebSocket>();
    const [displayMode, setDisplayMode] = useState("0");
    const [use24HourFormat, setUse24HourFormat] = useState(0);
    const [scrollingSpeed, setScrollingSpeed] = useState(8);
    const [messageHoldTime, setMessageHoldTime] = useState(6000);
    const [scrollingMessage, setScrollingMessage] = useState("");
    const [scrollMessageError, setScrollMessageError] = useState<null | string>(null);
    const [date, setDate] = useState("");
    const handleScrollingMessageUpdate = () => {
        if (!checkScrollMessage(scrollingMessage)) return;
        ws?.send(MessageType.SET_TEXT + " " + scrollingMessage);
    };

    const checkScrollMessage = (message: string) => {
        if (message.length <= 5) {
            setScrollMessageError("Please enter a message up to file character long");
            return false;
        } else {
            setScrollMessageError(null);
        }
        return true;
    };
    const [wsState, setWsState] = useState<{
        connected: boolean;
        close: boolean;
    }>({
        connected: false,
        close: false,
    });
    useEffect(() => {
        const socket = new WebSocket(`ws://${ip}/ws`);
        socket.addEventListener("open", () => {
            setWsState({
                connected: true,
                close: false,
            });
        });

        socket.addEventListener("close", () => {
            setWsState({
                connected: false,
                close: true,
            });
        });

        socket.addEventListener("message", (event) => {
            const message = event.data;
            try {
                const data: {
                    displayMode: string;
                    use24HourFormat: string;
                    scrollingSpeed: string;
                    messageHoldTime: string;
                    scrollingMessage: string;
                } = JSON.parse(message) as any;
                console.log(data);
                setDisplayMode(data.displayMode);
                setUse24HourFormat(Number(data.use24HourFormat));
                setScrollingSpeed(Number(data.scrollingSpeed));
                setMessageHoldTime(Number(data.messageHoldTime));
                setScrollingMessage(data.scrollingMessage);
            } catch (error) {
                console.error("error while converting the message");
            }
        });

        setWs(socket);
        return () => {
            socket.close();
        };
    }, []);

    if (!wsState.connected)
        return (
            <div className="flex justify-center items-center">
                <p className="text-white">Loading..</p>
            </div>
        );
    const labels = [
        { value: "0", label: "Time" },
        { value: "1", label: "Date" },
        { value: "2", label: "Temperature" },
        { value: "3", label: "Humidity" },
        { value: "4", label: "Animation" },
        { value: "5", label: "Loop" },
    ];
    return (
        <div className="container max-w-lg px-3">
            <div className="flex flex-col gap-3">
                <div className="rounded-xl border border-mine-shaft-600 p-4 bg-mine-shaft-800">
                    <h2 className="text-white text-xl mb-3">Change Display Mode</h2>

                    <div className="flex flex-wrap gap-x-3 gap-y-0">
                        {labels.map((label) => (
                            <div className="flex items-center mb-4 cursor-pointer" key={label.value}>
                                <input
                                    id={`id-${label.value}`}
                                    type="radio"
                                    value={label.value}
                                    checked={label.value === displayMode}
                                    onChange={(e) => {
                                        ws?.send(MessageType.MODE + " " + e.currentTarget.value);
                                        setDisplayMode(e.currentTarget.value);
                                    }}
                                    name="default-radio"
                                    className="w-4 h-4 bg-m-green-700 border-m-green-600 accent-m-green-600"
                                />
                                <label htmlFor={`id-${label.value}`} className="select-none pl-2 text-sm font-medium text-gray-900 dark:text-gray-300">
                                    {label.label}
                                </label>
                            </div>
                        ))}
                    </div>
                </div>
                <div className="rounded-xl border border-mine-shaft-600 p-4 bg-mine-shaft-800">
                    <h2 className="text-white text-xl mb-3">Display Settings</h2>
                    <div className="flex flex-col gap-4">
                        <div className="flex items-center justify-between">
                            <input
                                type="datetime-local"
                                value={date}
                                onChange={(a) => {
                                    console.log(a.currentTarget.value);
                                    if (!a.currentTarget.value) return;
                                    setDate(a.currentTarget.value);
                                }}
                                placeholder="Pick date and time"
                                className={`text-xs active:outline-none px-3 py-2 max-w-xs rounded-lg focus:outline-none border-2 border-m-green-500 focus:border-m-green-400 text-white bg-transparent`}
                            />
                            <button
                                className="py-2 px-3 border-2 border-m-green-600 active:border-m-green-500 rounded-lg text-white"
                                onClick={() => {
                                    const d = new Date(date);
                                    const year = d.getFullYear();
                                    const month = d.getMonth();
                                    +1;
                                    const day = d.getDate();
                                    const hour = d.getHours();
                                    const minutes = d.getMinutes();
                                    ws?.send(MessageType.SET_DATE_TIME + ` ${year},${month},${day},${hour},${minutes},${0}`);
                                }}
                            >
                                Change Date
                            </button>
                        </div>
                        <div className="flex items-center justify-between">
                            <label>Use 24-hour format</label>
                            <input
                                className="w-5 h-5  accent-m-green-600"
                                type="checkbox"
                                checked={use24HourFormat ? true : false}
                                onChange={(e) => {
                                    ws?.send(MessageType.SET_DATE_FORMAT + " " + (e.currentTarget.checked ? "1" : "0"));
                                    setUse24HourFormat(e.currentTarget.checked ? 1 : 0);
                                }}
                            />
                        </div>
                        {/* <Group position="apart">
                            <Text>Animation speed</Text>
                            <CustomNumberInput
                                max={100}
                                value={scrollingSpeed}
                                onValueChange={(val) => {
                                    if (val !== scrollingSpeed) {
                                        setScrollingSpeed(val);
                                        ws?.send(MessageType.SET_ANIMATION_SPEED + " " + val);
                                    }
                                }}
                            />
                        </Group>
                        <Group position="apart">
                            <Text>Stay time</Text>
                            <CustomNumberInput
                                max={15000}
                                min={5000}
                                step={1000}
                                value={messageHoldTime}
                                onValueChange={(val) => {
                                    if (val !== messageHoldTime) {
                                        setMessageHoldTime(val);
                                        ws?.send(MessageType.SET_STAY_TIME + " " + val);
                                    }
                                }}
                            />
                        </Group>
                        <Textarea
                            placeholder="Your message"
                            label="Your message"
                            withAsterisk
                            value={scrollingMessage}
                            onChange={(e) => {
                                checkScrollMessage(e.currentTarget.value);
                                setScrollingMessage(e.currentTarget.value);
                            }}
                            error={scrollMessageError}
                        />
                        <Group position="right">
                            <Button variant="outline" onClick={handleScrollingMessageUpdate}>
                                Update
                            </Button>
                        </Group> */}
                    </div>
                </div>
            </div>
        </div>
    );
};

// const CustomNumberInput: React.FC<{ max?: number; min?: number; step?: number; value?: number; onValueChange?: (value: any) => void; time?: number }> = ({ max = 10, min = 0, step = 2, onValueChange, time = 350, value }) => {
//     const [_value, setValue] = useState<number>(value || min);
//     const handlers = useRef<NumberInputHandlers>();

//     useEffect(() => {
//         setValue(value || min);
//     }, [value]);

//     return (
//         <Group spacing={5}>
//             <ActionIcon size={36} variant="default" onClick={() => handlers?.current?.decrement()}>
//                 –
//             </ActionIcon>

//             <input
//                                 type="number"

//                                 placeholder="Pick date and time"
//                                 className={`text-xs active:outline-none px-3 py-2 max-w-xs rounded-lg focus:outline-none border-2 border-m-green-500 focus:border-m-green-400 text-white bg-transparent`}
//                 readOnly
//                 onChange={(val) => {
//                     setValue(val as number);
//                     onValueChange && onValueChange(val);
//                 }}
//                 handlersRef={handlers}
//                 value={_value}
//                 max={max}
//                 min={min}
//                 step={step}
//                 styles={{
//                     input: {
//                         width: rem(64),
//                         textAlign: "center",
//                     },
//                 }}
//             />

//             <ActionIcon size={36} variant="default" onClick={() => handlers?.current?.increment()}>
//                 +
//             </ActionIcon>
//         </Group>
//     );
// };
