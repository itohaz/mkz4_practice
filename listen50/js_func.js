var milkcocoa = new MilkCocoa('appid.mlkcca.com');

function send(command) {
    milkcocoa.dataStore('data').send({'com':command});
}
