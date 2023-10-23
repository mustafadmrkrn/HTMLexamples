const hoursSelect = document.getElementById("hours");
const minutesSelect = document.getElementById("minutes");
const secondsSelect = document.getElementById("seconds");
const startButton = document.getElementById("start");
const stopButton = document.getElementById("stop");
const countdownElement = document.getElementById("countdown");

let countdownTimer;

function updateCountdownDisplay(hours, minutes, seconds) {
    countdownElement.textContent = `${hours}:${minutes}:${seconds}`;
}

function startCountdown() {
    const hours = parseInt(hoursSelect.value) || 0;
    const minutes = parseInt(minutesSelect.value) || 0;
    const seconds = parseInt(secondsSelect.value) || 0;

    if (hours === 0 && minutes === 0 && seconds === 0) {
        return;
    }

    startButton.disabled = true;
    hoursSelect.disabled = true;
    minutesSelect.disabled = true;
    secondsSelect.disabled = true;

    let totalSeconds = hours * 3600 + minutes * 60 + seconds;

    countdownTimer = setInterval(() => {
        if (totalSeconds <= 0) {
            stopCountdown();
        } else {
            const h = Math.floor(totalSeconds / 3600);
            const m = Math.floor((totalSeconds % 3600) / 60);
            const s = totalSeconds % 60;

            updateCountdownDisplay(
                h.toString().padStart(2, '0'),
                m.toString().padStart(2, '0'),
                s.toString().padStart(2, '0')
            );

            totalSeconds--;
        }
    }, 1000);
}

function stopCountdown() {
    clearInterval(countdownTimer);
    startButton.disabled = false;
    hoursSelect.disabled = false;
    minutesSelect.disabled = false;
    secondsSelect.disabled = false;
}

startButton.addEventListener("click", startCountdown);
stopButton.addEventListener("click", stopCountdown);
