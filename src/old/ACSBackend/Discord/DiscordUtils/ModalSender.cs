using BKDijitalYoklamaBackend.Discord;
using DSharpPlus;
using DSharpPlus.Entities;
using DSharpPlus.Interactivity.Extensions;

namespace ACSBackend.Discord.DiscordUtils
{
    public class ModalSender
    {
        public string Title { get; private set; }
        private DiscordClient Client;
        private List<TextInputComponent> _components = new();
        public IReadOnlyList<TextInputComponent> Components => _components.AsReadOnly();
        private Dictionary<string, string> result = new();

        public ModalSender(DiscordClient client, string title)
        {
            Title = title;
            Client = client;
        }


        public void AddInput(string id, string label, string? placeholder = null, TextInputStyle style = TextInputStyle.Short, string? value = null, bool required = true, int minLength = 0, int? maxLength = null)
        {
            _components.Add(new TextInputComponent(label, id, placeholder, value, required, style, minLength, maxLength));
        }


        public async Task<bool> Send(DiscordInteraction interaction, TimeSpan? timeoutOverride = null)
        {
            string modalID = "m" + interaction.Id;

            var interResp = new DiscordInteractionResponseBuilder()
                .WithTitle(Title)
                .WithCustomId(modalID);

            foreach (var comp in _components)
            {
                interResp = interResp.AddComponents(comp);
            }

            await interaction.CreateResponseAsync(DSharpPlus.InteractionResponseType.Modal, interResp);


            var resp = await Client.GetInteractivity().WaitForModalAsync(modalID, timeoutOverride);

            if (resp.TimedOut) { result = new(); return false; }

            result = resp.Result.Values.ToDictionary(x => x.Key, x => x.Value);


            await resp.Result.Interaction.CreateResponseAsync(DSharpPlus.InteractionResponseType.DeferredChannelMessageWithSource);

            return true;
        }

        public string this[string index]
        {
            get => result[index];
        }
    }
}
