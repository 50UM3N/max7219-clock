/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    container: { center: true },
    extend: {
      colors: {
        "m-green": {
          50: "#f5f8f6",
          100: "#ddeae2",
          200: "#bbd4c4",
          300: "#92b6a1",
          400: "#6b967f",
          500: "#517b65",
          600: "#3f6250",
          700: "#344e41",
          800: "#2e4138",
          900: "#293831",
          950: "#141f1a",
        },
        "mine-shaft": {
          50: "#f7f7f7",
          100: "#e3e3e3",
          200: "#c8c8c8",
          300: "#a4a4a4",
          400: "#818181",
          500: "#666666",
          600: "#515151",
          700: "#434343",
          800: "#383838",
          900: "#262626",
          950: "#1a1a1a",
        },
      },
    },
  },
  plugins: [],
}

