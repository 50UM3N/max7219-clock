import { ActionIcon, Button, Center, Chip, Container, Divider, Group, Header, Input, Loader, NumberInput, NumberInputHandlers, Paper, Stack, Switch, Text, Textarea, Title, rem, useMantineColorScheme, useMantineTheme } from "@mantine/core";
import { useDebouncedValue } from "@mantine/hooks";
import { IconMoonStars, IconPlugConnectedX, IconSun, IconWifi } from "@tabler/icons-react";
import { useEffect, useRef, useState } from "react";

enum MessageType {
    MODE = "0",
    SET_TIME = "1",
    SET_DATE = "2",
    SET_DATE_FORMAT = "3",
    SET_ANIMATION_SPEED = "4",
    SET_STAY_TIME = "5",
    SET_TEXT = "6",
}

function App() {
    const { colorScheme, toggleColorScheme } = useMantineColorScheme();
    const theme = useMantineTheme();
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
            <Header height={56} mb="md" style={{ display: "flex", alignItems: "center" }}>
                <Container w="100%" size="xs">
                    <Group position="apart">
                        <Title order={4}>Matrix Display</Title>
                        <Group>
                            <Group position="center">
                                <Switch
                                    checked={colorScheme === "dark"}
                                    onChange={() => toggleColorScheme()}
                                    size="lg"
                                    onLabel={<IconSun color={theme.white} size="1.25rem" stroke={1.5} />}
                                    offLabel={<IconMoonStars color={theme.colors.gray[6]} size="1.25rem" stroke={1.5} />}
                                />
                            </Group>
                            {openApp && (
                                <ActionIcon
                                    color="teal"
                                    radius="xl"
                                    variant="outline"
                                    onClick={() => {
                                        setOpenApp(false);
                                    }}
                                >
                                    <IconPlugConnectedX size="1.125rem" />
                                </ActionIcon>
                            )}
                        </Group>
                    </Group>
                </Container>
            </Header>
            {!openApp ? (
                <Container size="xs">
                    <Paper radius="md" withBorder p="md" pt="sm">
                        <Title order={3}>Welcome in Matrix Clock</Title>
                        <Divider my="xs" />
                        <Text mb="xs">Connect with</Text>
                        <Stack spacing="xs">
                            <Input
                                size="md"
                                icon={<IconWifi />}
                                placeholder="eg. 192.168.0.102"
                                title="IP Address"
                                value={ipInput}
                                onChange={(e) => {
                                    checkIpAddress(e.currentTarget.value);
                                    setIpInput(e.currentTarget.value);
                                }}
                                error={ipError}
                            />
                            <Group spacing="xs" grow>
                                <Button size="md" onClick={handleConnectToIp}>
                                    Connect to Ip
                                </Button>
                                <Button
                                    size="md"
                                    variant="outline"
                                    onClick={() => {
                                        setIpInput("");
                                        setOpenApp(true);
                                    }}
                                >
                                    Connect Default Ip
                                </Button>
                            </Group>
                        </Stack>
                    </Paper>
                </Container>
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
            <Center my="xl">
                <Loader />
            </Center>
        );

    return (
        <Container size="xs">
            <Stack spacing="">
                <Paper radius="md" withBorder p="md" pt="sm">
                    <Title order={4}>Change Display Mode</Title>
                    <Divider my="xs" />
                    <Chip.Group
                        multiple={false}
                        value={String(displayMode)}
                        onChange={(value) => {
                            ws?.send(MessageType.MODE + " " + value);
                            setDisplayMode(value);
                        }}
                    >
                        <Group position="center" spacing="xs">
                            <Chip size="md" value="0">
                                Time
                            </Chip>
                            <Chip size="md" value="1">
                                Date
                            </Chip>
                            <Chip size="md" value="2">
                                Temperature
                            </Chip>
                            <Chip size="md" value="3">
                                Humidity
                            </Chip>
                            <Chip size="md" value="4">
                                Animation
                            </Chip>
                            <Chip size="md" value="5">
                                Loop
                            </Chip>
                        </Group>
                    </Chip.Group>
                </Paper>
                <Paper radius="md" withBorder p="md" pt="sm">
                    <Title order={4}>Display Settings</Title>
                    <Divider my="xs" />
                    <Stack spacing="md" mt="md">
                        <Group position="apart">
                            <Text>9.45 AM</Text>
                            <Button variant="outline">Change Time</Button>
                        </Group>
                        <Group position="apart">
                            <Text>April 11, 2023</Text>
                            <Button variant="outline" onClick={() => ws?.send(MessageType.SET_TEXT + " hello world")}>
                                Change Date
                            </Button>
                        </Group>
                        <Group position="apart">
                            <Text>Use 24-hour format</Text>
                            <Switch
                                checked={use24HourFormat ? true : false}
                                onChange={(e) => {
                                    ws?.send(MessageType.SET_DATE_FORMAT + " " + (e.currentTarget.checked ? "1" : "0"));
                                    setUse24HourFormat(e.currentTarget.checked ? 1 : 0);
                                }}
                            />
                        </Group>
                        <Divider />
                        <Group position="apart">
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
                        <Divider />
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
                        </Group>
                    </Stack>
                </Paper>
            </Stack>
        </Container>
    );
};

const CustomNumberInput: React.FC<{ max?: number; min?: number; step?: number; value?: number; onValueChange?: (value: any) => void; time?: number }> = ({ max = 10, min = 0, step = 2, onValueChange, time = 350, value }) => {
    const [_value, setValue] = useState<number>(value || min);
    const handlers = useRef<NumberInputHandlers>();

    useEffect(() => {
        setValue(value || min);
    }, [value]);

    return (
        <Group spacing={5}>
            <ActionIcon size={36} variant="default" onClick={() => handlers?.current?.decrement()}>
                –
            </ActionIcon>

            <NumberInput
                readOnly
                hideControls
                onChange={(val) => {
                    setValue(val as number);
                    onValueChange && onValueChange(val);
                }}
                handlersRef={handlers}
                value={_value}
                max={max}
                min={min}
                step={step}
                styles={{
                    input: {
                        width: rem(64),
                        textAlign: "center",
                    },
                }}
            />

            <ActionIcon size={36} variant="default" onClick={() => handlers?.current?.increment()}>
                +
            </ActionIcon>
        </Group>
    );
};
