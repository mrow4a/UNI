# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0023_auto_20150809_1930'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='lamp_errors',
            name='lamp_number',
        ),
        migrations.AddField(
            model_name='lamp_errors',
            name='lamp_location',
            field=models.ForeignKey(to='parking.Lamp', null=True),
        ),
        migrations.AddField(
            model_name='lamp_errors',
            name='timestamp',
            field=models.DateTimeField(default=django.utils.timezone.now, blank=True),
        ),
    ]
